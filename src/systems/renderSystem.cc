#include "renderSystem.h"

RenderSystem::RenderSystem() {
  RenderSystem::renderer = new Renderer();
  // RenderSystem::camera = new Camera();
  RenderSystem::quatCamera = new QuatCamera();

  // move back the camera a little bit.
  quatCamera->TranslateInWorld(glm::vec3(0.0f, 1.0f, 35.0f));

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

  // DEBUG START
  // static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

  // static float snap[3] = {1.f, 1.f, 1.f};

  // ImGui_ImplOpenGL3_NewFrame();
  // ImGui_ImplGlfw_NewFrame();
  // ImGui::NewFrame();

  // ImGui::Begin("Hello");

  // if (mCurrentGizmoOperation == ImGuizmo::TRANSLATE) {
  //   ImGui::InputFloat3("Snap", &snap[0]);
  // }

  // ImGui::End();

  // ImGui::Render();
  // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  // DEBUG END
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
  if (!input->mouseKeys[MOUSE_LEFT]) {
    return;
  }

  // Turn this off for this frame so that it doesn't generate hundreds of rays
  input->mouseKeys[MOUSE_LEFT] = false;
  DevDebug& devDebug = registry->GetComponent<DevDebug>();

  glm::vec3 rayDirection = GetRayDirection(registry, input);

  Line* line = registry->GetComponent<Line>(devDebug.rayID);
  line->origin = quatCamera->GetPositionInWorld();
  line->destination = rayDirection;
  line->color.r = 1.0f;

  std::vector<Entity> entityIds;
  std::vector<BoundingBox> boundingBoxes;
  // Calculate all the positions, assume that there is a BoundingBoxCube around it.
  registry->GetComponentsIter<Transform>()->EachWithID([&](Entity id, Transform& transform) {
    if (transform.scale.x == 1.0f) {
      // Calculate the model matrix
      glm::mat4 matrixModel = glm::mat4(1.0f);

      matrixModel = glm::translate(matrixModel, transform.position);
      matrixModel = glm::scale(matrixModel, transform.scale);

      std::vector<glm::vec4> verticesCollection;
      verticesCollection.reserve(boundingBoxCubeVertices.size() / 3);

      for (size_t i = 0; i < boundingBoxCubeVertices.size(); i += 3) {
        verticesCollection.push_back(matrixModel * glm::vec4(boundingBoxCubeVertices[i],
                                                             boundingBoxCubeVertices[i + 1],
                                                             boundingBoxCubeVertices[i + 2], 1.0f));
      }

      BoundingBox bb = GetBoundingBox(verticesCollection);
      boundingBoxes.push_back(bb);

      entityIds.push_back(id);
    }
  });

  // Calculate the distance between the ray origin and the bounding box?
  auto origin = quatCamera->GetPositionInWorld();

  // Resolve the collision if there are more than one intersections / collisions
  // TODO : Need a way to get the ID from registry->GetComponents<Iter>()
  // Perhaps overload the GetComponents such that you can also take in
  // Each([](uint32_t id, Transform& transform))
  std::vector<float> lengths;
  std::vector<uint32_t> lengthIndexs;
  for (size_t i = 0; i < boundingBoxes.size(); i++) {
    auto collisionAndLength = RayBoundingBoxCollisionCheck(origin, rayDirection, boundingBoxes[i]);
    if (std::get<bool>(collisionAndLength)) {
      lengths.push_back(std::get<float>(collisionAndLength));
      lengthIndexs.push_back(i);
    }
  }

  if (lengths.size() == 1) {
    lucid::Log("Single collision! ID : ", entityIds[lengthIndexs[0]]);
  }

  // If it has collided with more than one object
  // uint32_t shortestIndex = 0;
  // float shortestLength = std::numeric_limits<float>::max();
  // for (size_t i = 0; i < lengths.size(); i++) {
  //   if (lengths[i] < shortestLength) {
  //     shortestLength = lengths[i];
  //     shortestIndex = lengthIndexs[i];
  //   }
  // }

  // Entity id = registry->GetEntityIDFromArchetype<Transform>(shortestIndex);

  // lucid::Log("ID : ", id);

  // if (lengths.size() > 1) {
  //   lucid::Log("Collided with more than one object, number of objects : ", lengths.size());
  //   for (size_t i = 0; i < lengths.size(); i++) {
  //     lucid::Log(i, " : ", lengths[i]);
  //   }
  // }
}

void RenderSystem::DrawAllLines(double dt, Registry* registry, Input* input) {
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();
  DevDebug devDebug = registry->GetComponent<DevDebug>();

  shaderResource.primitiveShader.Bind();
  shaderResource.primitiveShader.SetUniformMatFloat4("projection", quatCamera->GetProjection());
  shaderResource.primitiveShader.SetUniformMatFloat4("view", quatCamera->GetView());

  registry->GetComponentsIter<Line, Transform>()->Each([dt, &shaderResource, &renderer = renderer,
                                                        &quatCamera = quatCamera](
                                                           Line& line, Transform& transform) {
    // Update the position to the origin of the transform
    transform.position = line.origin;
    // The scale is the 'direction' on how far to move
    transform.scale = line.destination - line.origin;

    glm::mat4 matrixModel = glm::mat4(1.0f);
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    matrixModel = glm::translate(matrixModel, transform.position);
    matrixModel = glm::scale(matrixModel, transform.scale);

    // Rotation matrix
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

    matrixModel *= rotationMatrix;

    // Debug START
    // matrixModel = quatCamera->GetPosition();
    // Debug END

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

glm::vec3 RenderSystem::GetRayDirection(Registry* registry, Input* input) {
  DevDebug& devDebug = registry->GetComponent<DevDebug>();

#if DEBUG
  float mouseX = static_cast<float>(input->GetMouseX() - devDebug.leftWindowWidth);
  float mouseY = static_cast<float>(input->GetMouseYAbsolute() - devDebug.menuBarHeight);

  // lucid::Log("x : ", input->GetMouseX(), " y : ", input->GetMouseYAbsolute())

  float x =
      (2.0f * mouseX) / (SCREEN_WIDTH - devDebug.leftWindowWidth - devDebug.rightWindowWidth) -
      1.0f;
  float y = 1.0f - (2.0f * mouseY) /
                       (SCREEN_HEIGHT - devDebug.bottomWindowHeight - devDebug.menuBarHeight);
  float z = 1.0f;
#endif

#if RELEASE
  float mouseX = static_cast<float>(input->GetMouseX());
  float mouseY = static_cast<float>(input->GetMouseYAbsolute());

  float x = (2.0f * mouseX) / SCREEN_WIDTH - 1.0f;
  float y = 1.0f - (2.0f * mouseY) / SCREEN_HEIGHT;
  float z = 1.0f;
#endif

  // lucid::Log(x, " ", y, " ", z);

  // normalized device coordinates
  glm::vec3 rayNds = glm::vec3(x, y, z);

  // homogeneous clip coordinates
  glm::vec4 rayClip = glm::vec4(rayNds.x, rayNds.y, -1.0f, 1.0f);

  // convert to eye/camera coordinates
  glm::vec4 rayEye = glm::inverse(quatCamera->GetProjection()) * rayClip;

  // unproject the x, z part
  rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

  // 4d world coordinates
  // normalize the vector as well
  glm::vec3 rayWorld = glm::vec3(glm::normalize(glm::inverse(quatCamera->GetView()) * rayEye));
  // lucid::Log(glm::to_string(rayWorld));

  // Scale this by a fairly huge amount
  rayWorld *= 1000.0f;

  // Re-inverse the y-values since input->GetMouseY() {abs(SCREEN_HEIGHT - y)}
  // TODO : Find out why i have to invert this for whatever reason...

  // rayWorld.x = -rayWorld.x;
  // rayWorld.y = rayWorld.y;
  // rayWorld.z = -15.0f;

  return rayWorld;
}

// bool : true if collided, false if not collided
// float : length of the intersection from origin if it is intersected
std::tuple<bool, float> RenderSystem::RayBoundingBoxCollisionCheck(glm::vec3 origin, glm::vec3 ray,
                                                                   BoundingBox boundingBox) {
  float length;
  glm::vec3 dirfrac = 1.0f / ray;
  // dirfrac.x = 1.0f / ray.x;
  // dirfrac.y = 1.0f / ray.y;
  // dirfrac.z = 1.0f / ray.z;

  float t1 = (boundingBox.minX - origin.x) * dirfrac.x;
  float t2 = (boundingBox.maxX - origin.x) * dirfrac.x;

  float t3 = (boundingBox.minY - origin.y) * dirfrac.y;
  float t4 = (boundingBox.maxY - origin.y) * dirfrac.y;

  float t5 = (boundingBox.minZ - origin.z) * dirfrac.z;
  float t6 = (boundingBox.maxZ - origin.z) * dirfrac.z;

  float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
  float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

  // AABB is behind
  if (tmax < 0) {
    // lucid::Log("AABB is behind");
    // return false;
    return std::tuple(false, 0.0f);
  }

  // Does not intersect
  if (tmin > tmax) {
    // lucid::Log("Does not intersect");
    // return false;
    return std::tuple(false, 0.0f);
  }

  length = tmin;
  return std::tuple(true, length);
  // return true;
}

BoundingBox RenderSystem::GetBoundingBox(std::vector<glm::vec4> vertices) {
  BoundingBox bb;
  for (size_t i = 0; i < vertices.size(); i++) {
    bb.minX = glm::min(vertices[i].x, bb.minX);
    bb.maxX = glm::max(vertices[i].x, bb.maxX);

    bb.minY = glm::min(vertices[i].y, bb.minY);
    bb.maxY = glm::max(vertices[i].y, bb.maxY);

    bb.minZ = glm::min(vertices[i].z, bb.minZ);
    bb.maxZ = glm::max(vertices[i].z, bb.maxZ);
  }
  return bb;
}