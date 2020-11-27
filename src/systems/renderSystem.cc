#include "renderSystem.h"

RenderSystem::RenderSystem() {
  RenderSystem::renderer = new Renderer();
  // RenderSystem::camera = new Camera();
  RenderSystem::quatCamera = new QuatCamera();

  // move back the camera a little bit.
  quatCamera->Translate(glm::vec3(0.0f, 0.0f, -10.0f));

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
  HandleMousePick(dt, registry, input);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ShaderResource& shaderResource = registry->GetComponent<ShaderResource>();

  SceneRender& sceneRender = registry->GetComponent<SceneRender>();
  sceneRender.textureID = texture;

  DevDebug& devDebug = registry->GetComponent<DevDebug>();

  // TODO : wireframe drawing should have its own shaders
  // Draw wireframe
  // glLineWidth(3.0f);
  if (devDebug.drawWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  DrawAllLines(dt, registry, input);
  DrawAllModels(dt, registry, input);
  DrawAllCubes(dt, registry, input);
  DrawAllSpheres(dt, registry, input);

  if (devDebug.drawColliders) DrawAllBoundingBoxes(dt, registry, input);
  if (devDebug.drawWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderSystem::HandleMousePan(double dt, Input* input) {
  if (input->IsMouseLDown() &&                //
      (input->lastX != input->GetMouseX() ||  //
       input->lastY != input->GetMouseY())) {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    // These are simple movements that can just be translated on the spot
    // Handle the x-axis movement
    quatCamera->Translate(glm::vec3((offsetX * dt), 0.0f, 0.0f));

    // Handle the y-axis movement
    quatCamera->Translate(glm::vec3(0.0f, offsetY * dt, 0.0f));

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();
    return;
  }

  if (input->IsMouseRDown() &&                //
      (input->lastX != input->GetMouseX() ||  //
       input->lastY != input->GetMouseY())) {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();

    // camera->UpdateCameraVector(offsetX, offsetY);
    quatCamera->PanCamera(dt, offsetX, offsetY);
    return;
  }
}

void RenderSystem::HandleMouseScroll(double dt, Input* input) {
  // Scroll up
  if (input->GetScrollState() == 1) {
    quatCamera->Translate(glm::vec3(0.0f, 0.0f, SCROLL_SPEED * dt));
  }

  // Scroll down
  if (input->GetScrollState() == -1) {
    quatCamera->Translate(glm::vec3(0.0f, 0.0f, -(SCROLL_SPEED * dt)));
  }

  // Reset the scroll variable once done
  input->scroll = 0;
  return;
}

void RenderSystem::HandleKeyboardPan(double dt, Input* input) {
  if (input->IsKeyDown('W')) quatCamera->Translate(glm::vec3(0.0f, 0.0f, CAMERA_SPEED * dt));
  if (input->IsKeyDown('S')) quatCamera->Translate(glm::vec3(0.0f, 0.0f, -(CAMERA_SPEED * dt)));
  if (input->IsKeyDown('A')) quatCamera->Translate(glm::vec3(CAMERA_SPEED * dt, 0.0f, 0.0f));
  if (input->IsKeyDown('D')) quatCamera->Translate(glm::vec3(-(CAMERA_SPEED * dt), 0.0f, 0.0f));

  // Temporary : TODO : make this more usable; this can use modifier keys to be more accessible / do
  // more things
  const float PAN_SPEED = 5.0f;
  if (input->IsKeyDown(GLFW_KEY_LEFT)) quatCamera->PanCamera(dt, -PAN_SPEED, 0);
  if (input->IsKeyDown(GLFW_KEY_RIGHT)) quatCamera->PanCamera(dt, PAN_SPEED, 0);
  if (input->IsKeyDown(GLFW_KEY_UP)) quatCamera->PanCamera(dt, 0, PAN_SPEED);
  if (input->IsKeyDown(GLFW_KEY_DOWN)) quatCamera->PanCamera(dt, 0, -PAN_SPEED);
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

void RenderSystem::HandleMousePick(double dt, Registry* registry, Input* input) {
  if (!input->IsMouseLDown()) {
    return;
  }

  float mouseX = static_cast<float>(input->GetMouseX());
  float mouseY = static_cast<float>(input->GetMouseY());

  float x = (2.0f * mouseX) / SCREEN_WIDTH - 1.0f;
  float y = 1.0f - (2.0f * mouseY) / SCREEN_HEIGHT;
  float z = 1.0f;

  // normalized device coordinates
  glm::vec3 rayNds = glm::vec3(x, y, z);

  // homogeneous clip coordinates
  glm::vec4 rayClip = glm::vec4(rayNds.x, rayNds.y, -1.0f, 1.0f);

  // convert to eye/camera coordinates
  glm::vec4 rayEye = glm::inverse(quatCamera->GetProjection()) * rayClip;

  // unproject the x, z part
  rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 1.0f);

  // 4d world coordinates
  // normalize the vector as well
  auto inversed = glm::inverse(quatCamera->GetView());

  glm::vec3 rayWorld = glm::normalize(glm::vec3(inversed * rayEye));
  std::cout << glm::to_string(rayWorld) << std::endl;
}

void RenderSystem::DrawAllLines(double dt, Registry* registry, Input* input) {
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();
  DevDebug devDebug = registry->GetComponent<DevDebug>();

  shaderResource.primitiveShader.Bind();
  shaderResource.primitiveShader.SetUniformMatFloat4("projection", quatCamera->GetProjection());
  shaderResource.primitiveShader.SetUniformMatFloat4("view", quatCamera->GetView());

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
  shaderResource.modelShader.SetUniformMatFloat4("projection", quatCamera->GetProjection());
  shaderResource.modelShader.SetUniformMatFloat4("view", quatCamera->GetView());

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
  shaderResource.primitiveShader.SetUniformMatFloat4("projection", quatCamera->projection);
  shaderResource.primitiveShader.SetUniformMatFloat4("view", quatCamera->GetView());

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
  shaderResource.primitiveShader.SetUniformMatFloat4("projection", quatCamera->GetProjection());
  shaderResource.primitiveShader.SetUniformMatFloat4("view", quatCamera->GetView());

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

void RenderSystem::DrawAllBoundingBoxes(double dt, Registry* registry, Input* input) {
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();

  shaderResource.primitiveShader.Bind();
  shaderResource.primitiveShader.SetUniformMatFloat4("projection", quatCamera->GetProjection());
  shaderResource.primitiveShader.SetUniformMatFloat4("view", quatCamera->GetView());

  registry->GetComponentsIter<Transform, BoundingBoxCube>()->Each(
      [dt, &shaderResource, &renderer = renderer](Transform& transform,
                                                  BoundingBoxCube& boundingBoxCube) {
        glm::mat4 matrixModel = glm::mat4(1.0f);
        glm::mat4 rotationMatrix = glm::mat4(1.0f);

        matrixModel = glm::translate(matrixModel, transform.position);
        matrixModel = glm::scale(matrixModel, transform.scale);

        // Rotation matrix
        rotationMatrix =
            glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
        rotationMatrix =
            glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
        rotationMatrix =
            glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

        matrixModel *= rotationMatrix;

        shaderResource.primitiveShader.SetUniformMatFloat4("model", matrixModel);
        shaderResource.primitiveShader.SetUniformVecFloat3("uColor", boundingBoxCube.color);
        renderer->DrawBoundingBox(boundingBoxCube, shaderResource.primitiveShader);
      });

  shaderResource.primitiveShader.Unbind();
}