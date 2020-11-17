#include "renderSystem.h"

RenderSystem::RenderSystem() {
  RenderSystem::renderer = new Renderer();
  RenderSystem::camera = new Camera();

  // camera->MoveCamera(glm::vec3(-10.0f, 0.0f, 0.0f));

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // TODO : The SCREEN_WIDTH and SCREEN_HEIGHT here has to be changed
  // whenever the game is resized
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);

  // TODO : The SCREEN_WIDTH and SCREEN_HEIGHT here has to be changed
  // whenever the game is resized
  // Use renderbuffer for both (depth) and (stencil) buffer
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1440, 900);
  // Attach the render buffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderSystem::~RenderSystem() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &fbo);
}

void RenderSystem::Update(double dt, Registry* registry, Input* input) {
  HandleMousePan(dt, input);
  HandleMouseScroll(dt, input);
  HandleKeyboardPan(dt, input);
  HandleKeyboardInput(dt, registry, input);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera->UpdateView();

  ShaderResource& shaderResource = registry->GetComponent<ShaderResource>();

  SceneRender& sceneRender = registry->GetComponent<SceneRender>();
  sceneRender.textureID = texture;

  DevDebug& devDebug = registry->GetComponent<DevDebug>();

  devDebug.cameraUp = camera->cameraUp;
  devDebug.cameraFront = camera->cameraFront;
  devDebug.cameraPos = camera->cameraPos;

  // TODO : wireframe drawing should have its own shaders
  // Draw wireframe
  if (devDebug.drawWireframe) {
    // Set the lines to be drawn
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glLineWidth(3.0f);

    DrawAllModels(dt, registry, input);
    DrawAllCubes(dt, registry, input);
    DrawAllSpheres(dt, registry, input);

    // Reset it back to the default fill
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else {
    // Regular standard draw

    glLineWidth(3.0f);
    DrawAllLines(dt, registry, input);
    // DrawAllModels(dt, registry, input);
    // DrawAllCubes(dt, registry, input);
    // DrawAllSpheres(dt, registry, input);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderSystem::HandleMousePan(double dt, Input* input) {
  if (input->IsMouseLDown() &&
      (input->lastX != input->GetMouseX() || input->lastY != input->GetMouseY())) {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    // Handle the x-axis movement
    camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) *
                         static_cast<float>(offsetX * dt);

    // Handle the y-axis movement
    camera->cameraPos -= glm::normalize(camera->cameraUp) * static_cast<float>(offsetY * dt);

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();
    return;
  }

  if (input->IsMouseRDown() &&
      (input->lastX != input->GetMouseX() || input->lastY != input->GetMouseY())) {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();

    camera->UpdateCameraVector(offsetX, offsetY);
    return;
  }
}

void RenderSystem::HandleMouseScroll(double dt, Input* input) {
  // Scroll up
  if (input->GetScrollState() == 1) {
    camera->cameraPos += static_cast<float>(SCROLL_SPEED * dt) * camera->cameraFront;
  }

  // Scroll down
  if (input->GetScrollState() == -1) {
    camera->cameraPos -= static_cast<float>(SCROLL_SPEED * dt) * camera->cameraFront;
  }

  // Reset the scroll variable once done
  input->scroll = 0;
  return;
}

void RenderSystem::HandleKeyboardPan(double dt, Input* input) {
  if (input->IsKeyDown('w')) {
    camera->cameraPos += static_cast<float>(CAMERA_SPEED * dt) * camera->cameraFront;
  }

  if (input->IsKeyDown('s')) {
    camera->cameraPos -= static_cast<float>(CAMERA_SPEED * dt) * camera->cameraFront;
  }

  if (input->IsKeyDown('a')) {
    camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) *
                         static_cast<float>(CAMERA_SPEED * dt);
  }

  if (input->IsKeyDown('d')) {
    camera->cameraPos += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) *
                         static_cast<float>(CAMERA_SPEED * dt);
  }
}

// Note : temporary
void RenderSystem::HandleKeyboardInput(double dt, Registry* registry, Input* input) {
  if (input->IsKeyDown('1')) {
    Entity cubeID = registry->GetAvailableEntityId();

    registry->CreateEntity<Cube, Transform>(cubeID);

    Transform* transform = registry->GetComponent<Transform>(cubeID);
    transform->position = camera->cameraPos;

    input->SetKeyOff('1');
  }
}

void RenderSystem::DrawAllLines(double dt, Registry* registry, Input* input) {
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();
  DevDebug devDebug = registry->GetComponent<DevDebug>();

  shaderResource.primitiveShader.Bind();
  shaderResource.primitiveShader.SetUniformMatFloat4("projection", camera->projection);
  shaderResource.primitiveShader.SetUniformMatFloat4("view", camera->view);

  registry->GetComponentsIter<Line, Transform>()->Each([dt, &shaderResource, &renderer = renderer,
                                                        &devDebug](Line& line,
                                                                   Transform& transform) {
    glm::mat4 matrixModel = glm::mat4(1.0f);
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    matrixModel = glm::translate(matrixModel, transform.position);
    matrixModel = glm::scale(matrixModel, transform.scale);

    // Rotation matrix
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

    matrixModel *= rotationMatrix;

    shaderResource.primitiveShader.SetUniformMatFloat4("model", matrixModel);
    shaderResource.primitiveShader.SetUniformVecFloat3("uColor", line.color);
    renderer->DrawLine(line, shaderResource.primitiveShader);
  });

  shaderResource.primitiveShader.Unbind();
}

void RenderSystem::DrawAllModels(double dt, Registry* registry, Input* input) {
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();

  shaderResource.modelShader.Bind();
  shaderResource.modelShader.SetUniformMatFloat4("projection", camera->projection);
  shaderResource.modelShader.SetUniformMatFloat4("view", camera->view);

  registry->GetComponentsIter<Model, Transform>()->Each([dt, &shaderResource, &renderer = renderer](
                                                            Model& model, Transform& transform) {
    glm::mat4 matrixModel = glm::mat4(1.0f);
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    matrixModel = glm::translate(matrixModel, transform.position);
    matrixModel = glm::scale(matrixModel, transform.scale);

    // Rotation matrix
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

    matrixModel *= rotationMatrix;

    shaderResource.modelShader.SetUniformMatFloat4("model", matrixModel);
    renderer->DrawModel(model, shaderResource.modelShader);
  });

  shaderResource.modelShader.Unbind();
}

void RenderSystem::DrawAllCubes(double dt, Registry* registry, Input* input) {
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();
  DevDebug devDebug = registry->GetComponent<DevDebug>();

  shaderResource.primitiveShader.Bind();
  shaderResource.primitiveShader.SetUniformMatFloat4("projection", camera->projection);
  shaderResource.primitiveShader.SetUniformMatFloat4("view", camera->view);

  registry->GetComponentsIter<Cube, Transform>()->Each([dt, &shaderResource, &devDebug,
                                                        &renderer = renderer](
                                                           Cube& cube, Transform& transform) {
    glm::mat4 matrixModel = glm::mat4(1.0f);
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    matrixModel = glm::translate(matrixModel, transform.position);
    matrixModel = glm::scale(matrixModel, transform.scale);

    // Rotation matrix
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

    matrixModel *= rotationMatrix;

    shaderResource.primitiveShader.SetUniformMatFloat4("model", matrixModel);
    shaderResource.primitiveShader.SetUniformVecFloat3("uColor", cube.color);
    ;
    renderer->DrawCube(cube, shaderResource.primitiveShader);
  });

  shaderResource.primitiveShader.Unbind();
}

void RenderSystem::DrawAllSpheres(double dt, Registry* registry, Input* input) {
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();
  DevDebug devDebug = registry->GetComponent<DevDebug>();

  shaderResource.primitiveShader.Bind();
  shaderResource.primitiveShader.SetUniformMatFloat4("projection", camera->projection);
  shaderResource.primitiveShader.SetUniformMatFloat4("view", camera->view);

  registry->GetComponentsIter<Sphere, Transform>()->Each([dt, &shaderResource, &devDebug,
                                                          &renderer = renderer](
                                                             Sphere& sphere, Transform& transform) {
    glm::mat4 matrixModel = glm::mat4(1.0f);
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    matrixModel = glm::translate(matrixModel, transform.position);
    matrixModel = glm::scale(matrixModel, transform.scale);

    // Rotation matrix
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

    matrixModel *= rotationMatrix;

    shaderResource.primitiveShader.SetUniformMatFloat4("model", matrixModel);
    shaderResource.primitiveShader.SetUniformVecFloat3("uColor", sphere.color);
    renderer->DrawSphere(sphere, shaderResource.primitiveShader);
  });

  shaderResource.primitiveShader.Unbind();
}