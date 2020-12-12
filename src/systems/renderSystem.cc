#include "renderSystem.h"

RenderSystem::RenderSystem(Registry* registry)
{
  RenderSystem::renderer = new Renderer(registry);

  InitRenderBuffers();
  InitPrimitiveBuffers(registry);
  InitSceneCameraComponent(registry);
}

RenderSystem::~RenderSystem()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &fbo);
}

void RenderSystem::Update(float dt, Registry* registry, Input* input)
{
  // Temporary gateway for mouse picking
  HandleMousePick(dt, registry, input);
  HandleMousePan(dt, registry, input);
  HandleMouseScroll(dt, input);

  HandleKeyboardPan(dt, input);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void RenderSystem::InitRenderBuffers()
{
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // TODO : The SCREEN_WIDTH and SCREEN_HEIGHT here has to bechanged
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

void RenderSystem::InitSceneCameraComponent(Registry* registry)
{
  Entity cameraID = registry->GetAvailableEntityId();
  registry->RegisterArchetype<QuatCamera>();
  registry->CreateEntity<QuatCamera>(cameraID);

  // Just store a pointer to it as it is used very often
  // Translate the camera back a little bit so it makes more sense
  RenderSystem::quatCamera = registry->GetComponent<QuatCamera>(cameraID);
  quatCamera->TranslateInWorld({0.0f, 1.0f, 20.0f});
}

void RenderSystem::InitPrimitiveBuffers(Registry* registry)
{
  PrimitiveBatchIds& primitiveBatchIds = registry->GetComponent<PrimitiveBatchIds>();

  {  // Line buffers
    // Line : Generate VAO and VBO
    glGenVertexArrays(1, &primitiveBatchIds.lineVAO);
    glGenBuffers(1, &primitiveBatchIds.lineVBO);

    // Line : Bind VAO and VBO
    glBindVertexArray(primitiveBatchIds.lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, primitiveBatchIds.lineVBO);

    // Line : Set the BufferData to a max size with dynamic draw
    // glBufferData(GL_ARRAY_BUFFER, (MAX_BUFFER * sizeof(glm::mat4)) + (line_vertices.size() *
    // sizeof(float)), nullptr, GL_DYNAMIC_DRAW); glBufferData(GL_ARRAY_BUFFER,
    // (line_vertices.size()
    // * sizeof(float)) + (MAX_BUFFER * sizeof(glm::mat4)), nullptr, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, MAX_BUFFER * sizeof(LineVertex), nullptr, GL_DYNAMIC_DRAW);

    // Line : Update the vertex attributes
    std::size_t vec4Size = sizeof(glm::vec4);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                          (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                          (void*)(3 * sizeof(float) + (1 * vec4Size)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                          (void*)(3 * sizeof(float) + (2 * vec4Size)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                          (void*)(3 * sizeof(float) + (3 * vec4Size)));

    // Line : Set the matrix 4 divisors
    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    // Line : Un-Bind the Line VAO
    glBindVertexArray(0);
  }

  // TODO : Generate Sphere Buffers

  {  // Cube buffers
    glGenVertexArrays(1, &primitiveBatchIds.cubeVAO);
    glGenBuffers(1, &primitiveBatchIds.cubeVBO);
    glGenBuffers(1, &primitiveBatchIds.cubeEBO);

    // Cube : Bind VAO and VBO
    glBindVertexArray(primitiveBatchIds.cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, primitiveBatchIds.cubeVBO);

    // Cube : Set VBO data
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), &cubeVertices[0],
                 GL_STATIC_DRAW);

    // Cube : Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitiveBatchIds.cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(uint32_t), &cubeIndices[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
  }
}

void RenderSystem::HandleMousePan(float dt, Registry* registry, Input* input)
{
  DevDebug& devDebug = registry->GetComponent<DevDebug>();
  if (devDebug.onGizmo == true) return;

  if (input->IsMouseLDown() &&                //
      (input->lastX != input->GetMouseX() ||  //
       input->lastY != input->GetMouseY()))
  {
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
       input->lastY != input->GetMouseY()))
  {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();

    // camera->UpdateCameraVector(offsetX, offsetY);
    quatCamera->PanCamera(dt, offsetX, offsetY);
    return;
  }

  return;
}

void RenderSystem::HandleMouseScroll(float dt, Input* input)
{
  // Scroll up
  if (input->GetScrollState() == 1)
  {
    quatCamera->Translate(glm::vec3(0.0f, 0.0f, SCROLL_SPEED * dt));
  }

  // Scroll down
  if (input->GetScrollState() == -1)
  {
    quatCamera->Translate(glm::vec3(0.0f, 0.0f, -(SCROLL_SPEED * dt)));
  }

  // Reset the scroll variable once done
  input->scroll = 0;
  return;
}

void RenderSystem::HandleKeyboardPan(float dt, Input* input)
{
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

bool RenderSystem::HandleMousePick(float dt, Registry* registry, Input* input)
{
  if (!input->mouseKeys[MOUSE_LEFT])
  {
    return false;
  }

  if (input->activeWindow != WindowType::Scene)
  {
    return false;
  }

  // Turn this off for this frame so that it doesn't generate hundreds of rays
  input->mouseKeys[MOUSE_LEFT] = false;

  DevDebug& devDebug = registry->GetComponent<DevDebug>();
  glm::vec3 rayDirection = GetRayDirection(registry, input);

  // Don't really need this for now
  // Line* line = registry->GetComponent<Line>(devDebug.rayID);
  // line->origin = quatCamera->GetPositionInWorld();
  // line->destination = rayDirection;
  // line->color.r = 1.0f;

  std::vector<Entity> entityIds;
  std::vector<BoundingBox> boundingBoxes;
  // Calculate all the positions, assume that there is a BoundingBoxCube around it.
  registry->GetComponentsIter<Transform>()->EachWithID([&](Entity id, Transform& transform) {
    if (transform.scale.x == 1.0f)
    {
      // Calculate the model matrix
      glm::mat4 matrixModel = glm::mat4(1.0f);

      matrixModel = glm::translate(matrixModel, transform.position);
      matrixModel = glm::scale(matrixModel, transform.scale);

      std::vector<glm::vec4> verticesCollection;
      verticesCollection.reserve(boundingBoxCubeVertices.size() / 3);

      for (size_t i = 0; i < boundingBoxCubeVertices.size(); i += 3)
      {
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
  for (size_t i = 0; i < boundingBoxes.size(); i++)
  {
    auto collisionAndLength = RayBoundingBoxCollisionCheck(origin, rayDirection, boundingBoxes[i]);
    if (std::get<bool>(collisionAndLength))
    {
      lengths.push_back(std::get<float>(collisionAndLength));
      lengthIndexs.push_back(i);
    }
  }

  // If there are none collided
  if (lengths.size() == 0)
  {
    // If it does not collide, set activeEntity to be 0
    devDebug.activeEntity = 0;
    return false;
  }

  if (lengths.size() == 1)
  {
    // lucid::Log("Single collision! ID : ", entityIds[lengthIndexs[0]], " Length : ", lengths[0]);
    devDebug.activeEntity = entityIds[lengthIndexs[0]];

    // Change the focus to the inspector window
    devDebug.changeFocusWindow = WindowType::Inspector;
    return true;
  }

  // If it has collided with more than one object
  uint32_t shortestIndex = 0;
  float shortestLength = std::numeric_limits<float>::max();
  for (size_t i = 0; i < lengths.size(); i++)
  {
    if (lengths[i] < shortestLength)
    {
      shortestLength = lengths[i];
      shortestIndex = lengthIndexs[i];
    }
  }

  // Entity id = registry->GetEntityIDFromArchetype<Transform>(shortestIndex);

  // lucid::Log("Multiple collision! Shortest ID : ", entityIds[shortestIndex],
  //              " Length : ", shortestLength, " Amount : ", lengths.size());
  devDebug.activeEntity = entityIds[shortestIndex];

  // Change the focus to the inspector window
  devDebug.changeFocusWindow = WindowType::Inspector;

  return true;
}

void RenderSystem::DrawAllLines(float dt, Registry* registry, Input* input)
{
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();

  shaderResource.primitiveShaderBatch.Bind();
  shaderResource.primitiveShaderBatch.SetUniformMatFloat4("projection",
                                                          quatCamera->GetProjection());
  shaderResource.primitiveShaderBatch.SetUniformMatFloat4("view", quatCamera->GetView());

  renderer->StartBatch();

  registry->GetComponentsIter<Line, Transform>()->Each([&](Line& line, Transform& transform) {
    transform.position = line.origin;
    transform.scale = line.destination - line.origin;

    auto modelMatrix = GetModelMatrix(transform);
    renderer->PushLineBuffer(modelMatrix, line);
  });

  PrimitiveBatchIds& primitiveBatchIds = registry->GetComponent<PrimitiveBatchIds>();
  renderer->FlushBatch(primitiveBatchIds, DrawType::Line);

  shaderResource.primitiveShaderBatch.Unbind();
}

void RenderSystem::DrawAllModels(float dt, Registry* registry, Input* input)
{
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();

  shaderResource.modelShader.Bind();
  shaderResource.modelShader.SetUniformMatFloat4("projection", quatCamera->GetProjection());
  shaderResource.modelShader.SetUniformMatFloat4("view", quatCamera->GetView());

  registry->GetComponentsIter<Model, Transform>()->Each([&](Model& model, Transform& transform) {
    auto modelMatrix = GetModelMatrix(transform);

    shaderResource.modelShader.SetUniformMatFloat4("model", modelMatrix);
    renderer->DrawModel(model, shaderResource.modelShader);
  });

  shaderResource.modelShader.Unbind();
}

void RenderSystem::DrawAllCubes(float dt, Registry* registry, Input* input)
{
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();

  // shaderResource.primitiveShader.Bind();
  // shaderResource.primitiveShader.SetUniformMatFloat4("projection", quatCamera->projection);
  // shaderResource.primitiveShader.SetUniformMatFloat4("view", quatCamera->GetView());

  // registry->GetComponentsIter<Cube, Transform>()->Each([&](Cube& cube, Transform& transform) {
  //   auto modelMatrix = GetModelMatrix(transform);

  //   shaderResource.primitiveShader.SetUniformMatFloat4("model", modelMatrix);
  //   shaderResource.primitiveShader.SetUniformVecFloat3("uColor", cube.color);

  //   renderer->DrawCube(cube, shaderResource.primitiveShader);
  // });

  // shaderResource.primitiveShader.Unbind();

  shaderResource.cubeShaderBatch.Bind();
  shaderResource.cubeShaderBatch.SetUniformMatFloat4("projection", quatCamera->GetProjection());
  shaderResource.cubeShaderBatch.SetUniformMatFloat4("view", quatCamera->GetView());

  renderer->StartBatch();

  registry->GetComponentsIter<Cube, Transform>()->Each([&](Cube& cube, Transform& transform) {
    auto modelMatrix = GetModelMatrix(transform);
    renderer->PushCubeBuffer(modelMatrix, cube);
  });

  PrimitiveBatchIds primitiveBatchIds = registry->GetComponent<PrimitiveBatchIds>();
  renderer->FlushBatch(primitiveBatchIds, DrawType::Cube);

  shaderResource.cubeShaderBatch.Unbind();
}

void RenderSystem::DrawAllSpheres(float dt, Registry* registry, Input* input)
{
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();

  shaderResource.primitiveShader.Bind();
  shaderResource.primitiveShader.SetUniformMatFloat4("projection", quatCamera->GetProjection());
  shaderResource.primitiveShader.SetUniformMatFloat4("view", quatCamera->GetView());

  registry->GetComponentsIter<Sphere, Transform>()->Each([&](Sphere& sphere, Transform& transform) {
    auto modelMatrix = GetModelMatrix(transform);

    shaderResource.primitiveShader.SetUniformMatFloat4("model", modelMatrix);
    shaderResource.primitiveShader.SetUniformVecFloat3("uColor", sphere.color);
    renderer->DrawSphere(sphere, shaderResource.primitiveShader);
  });

  shaderResource.primitiveShader.Unbind();
}

void RenderSystem::DrawAllBoundingBoxes(float dt, Registry* registry, Input* input)
{
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();

  shaderResource.primitiveShader.Bind();
  shaderResource.primitiveShader.SetUniformMatFloat4("projection", quatCamera->GetProjection());
  shaderResource.primitiveShader.SetUniformMatFloat4("view", quatCamera->GetView());

  registry->GetComponentsIter<Transform, ColliderCube>()->Each(
      [&](Transform& transform, ColliderCube& colliderCube) {
        auto modelMatrix = GetModelMatrix(transform);

        shaderResource.primitiveShader.SetUniformMatFloat4("model", modelMatrix);
        shaderResource.primitiveShader.SetUniformVecFloat3("uColor", colliderCube.color);
        renderer->DrawBoundingBox(colliderCube, shaderResource.primitiveShader);
      });

  shaderResource.primitiveShader.Unbind();
}

glm::vec3 RenderSystem::GetRayDirection(Registry* registry, Input* input)
{
#if DEBUG
  WidgetLayout& widgetLayout = registry->GetComponent<WidgetLayout>();
  float mouseX = static_cast<float>(input->GetMouseX() - widgetLayout.leftWindowWidth);
  float mouseY = static_cast<float>(input->GetMouseYAbsolute() - widgetLayout.menuBarHeight -
                                    widgetLayout.topWindowHeight);

  // lucid::Log("x : ", input->GetMouseX(), " y : ", input->GetMouseYAbsolute())

  float x = (2.0f * mouseX) /
                (SCREEN_WIDTH - widgetLayout.leftWindowWidth - widgetLayout.rightWindowWidth) -
            1.0f;
  float y = 1.0f - (2.0f * mouseY) / (SCREEN_HEIGHT - widgetLayout.bottomWindowHeight -
                                      widgetLayout.menuBarHeight - widgetLayout.topWindowHeight);
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
                                                                   BoundingBox boundingBox)
{
  float length;
  glm::vec3 dirfrac = 1.0f / ray;

  float t1 = (boundingBox.minX - origin.x) * dirfrac.x;
  float t2 = (boundingBox.maxX - origin.x) * dirfrac.x;

  float t3 = (boundingBox.minY - origin.y) * dirfrac.y;
  float t4 = (boundingBox.maxY - origin.y) * dirfrac.y;

  float t5 = (boundingBox.minZ - origin.z) * dirfrac.z;
  float t6 = (boundingBox.maxZ - origin.z) * dirfrac.z;

  float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
  float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

  // AABB is behind
  if (tmax < 0)
  {
    return std::tuple(false, 0.0f);
  }

  // Does not intersect
  if (tmin > tmax)
  {
    return std::tuple(false, 0.0f);
  }

  length = tmin;
  return std::tuple(true, length);
}

BoundingBox RenderSystem::GetBoundingBox(std::vector<glm::vec4> vertices)
{
  BoundingBox bb;
  for (size_t i = 0; i < vertices.size(); i++)
  {
    bb.minX = glm::min(vertices[i].x, bb.minX);
    bb.maxX = glm::max(vertices[i].x, bb.maxX);

    bb.minY = glm::min(vertices[i].y, bb.minY);
    bb.maxY = glm::max(vertices[i].y, bb.maxY);

    bb.minZ = glm::min(vertices[i].z, bb.minZ);
    bb.maxZ = glm::max(vertices[i].z, bb.maxZ);
  }
  return bb;
}
