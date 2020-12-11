#include "uiSystem.h"

UiSystem::UiSystem()
{
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
  io.IniFilename = NULL;                             // Disable imgui.ini
  io.ConfigWindowsMoveFromTitleBarOnly |= ImGuiWindowFlags_NoMove;
  drawSceneOnly = false;
  (void)io;
}

UiSystem::~UiSystem() = default;

void UiSystem::Update(double dt, Registry* registry, Input* input)
{
  // Update the gizmo through input keys
  UpdateGizmoType(registry, input);

  InitializeGUI(dt, registry, input);
}

void UiSystem::InitializeGUI(double dt, Registry* registry, Input* input)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
  {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;

    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags host_window_flags = 0;
    host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoDocking;
    host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    char label[32];
    ImFormatString(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(label, NULL, host_window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockSpaceID = ImGui::GetID("DockSpace");
    if (!ImGui::DockBuilderGetNode(dockSpaceID))
    {
      PresetLayout(dockSpaceID);
    }

    ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), dockSpaceFlags);
    ImGui::End();

    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit"))
      {
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Help"))
      {
        ImGui::Text("WASD - Move the camera");
        ImGui::Text("Arrow Left/Right/Up/Down - Rotate Left/Right/Up/Down");
        ImGui::Text("J/L - [Demo] pong player movement");
        ImGui::Text("Mouse left - Pan Camera / [Debug] Shoot rays");
        ImGui::Text("Mouse right - Rotate Camera");

        ImGui::Text("1 : Translate Gizmo");
        ImGui::Text("2 : Rotation Gizmo");
        ImGui::Text("3 : Scale Gizmo");
        ImGui::EndMenu();
      }

      // TODO : Draw frametime
      // Draw FPS
      DevDebug& devDebug = registry->GetComponent<DevDebug>();
      std::string fpsCount = "FPS : " + std::to_string(devDebug.fps);
      ImGui::Text(fpsCount.c_str());

      ImGui::EndMainMenuBar();
    }
  }

  // Check whether to draw maximized scene
  UpdateSceneWindow(registry, input);
  if (!drawSceneOnly)
  {
    InitializeImGuiWindows(dt, registry, input);
  }
  else
  {
    DrawScene(dt, registry, input);
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UiSystem::PresetLayout(ImGuiID dockSpaceID)
{
  ImVec2 viewport_size = ImGui::GetMainViewport()->Size;
  ImVec2 viewport_pos = ImGui::GetMainViewport()->Pos;
  ImGui::DockBuilderRemoveNode(dockSpaceID);
  ImGui::DockBuilderAddNode(dockSpaceID, ImGuiDockNodeFlags_None);
  // TODO: set this programatically
  int menu_bar_height = 19;

  ImGuiID dockMainID = dockSpaceID;

  ImGuiID dockLeftID =
      ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Left, 0.2f, NULL, &dockMainID);
  ImGuiID dockRightID =
      ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Right, 0.2f, NULL, &dockMainID);
  ImGuiID dockBottomID =
      ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Down, 0.3f, NULL, &dockMainID);
  ImGuiID dockTopID =
      ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Up, 0.05f, NULL, &dockMainID);
  ImGuiID dockMiddleID =
      ImGui::DockBuilderSplitNode(dockTopID, ImGuiDir_Down, 0.95f, NULL, &dockTopID);
  ImGuiID dockBottomLeftID =
      ImGui::DockBuilderSplitNode(dockLeftID, ImGuiDir_Down, 0.3f, NULL, &dockLeftID);

  // Windows on left
  ImGui::DockBuilderDockWindow("Hierarchy", dockLeftID);
  // Windows on bottom left
  ImGui::DockBuilderDockWindow("Project", dockBottomLeftID);
  ImGui::DockBuilderDockWindow("Console", dockBottomLeftID);
  ImGui::DockBuilderDockWindow("Animation", dockBottomLeftID);
  ImGui::DockBuilderDockWindow("Animator", dockBottomLeftID);
  // Windows on right
  ImGui::DockBuilderDockWindow("Inspector", dockRightID);
  ImGui::DockBuilderDockWindow("Services", dockRightID);
  ImGui::DockBuilderDockWindow("DevDebug", dockRightID);
  // Windows in the middle
  ImGui::DockBuilderDockWindow("Scene", dockMiddleID);
  ImGui::DockBuilderDockWindow("Game Camera", dockMiddleID);
  // Windows on the top
  ImGui::DockBuilderDockWindow("ToolBar", dockTopID);
  // Windows on the bottom
  ImGui::DockBuilderDockWindow("Assets", dockBottomID);
  ImGui::DockBuilderDockWindow("Default Assets", dockBottomID);
  ImGui::DockBuilderFinish(dockSpaceID);
}

void UiSystem::InitializeImGuiWindows(double dt, Registry* registry, Input* input)
{
  DrawHierarchy(dt, registry, input);
  DrawAssets(dt, registry, input);
  DrawScene(dt, registry, input);
  DrawProject(dt, registry, input);
  DrawConsole(dt, registry, input);
  DrawAnimation(dt, registry, input);
  DrawAnimator(dt, registry, input);
  DrawInspector(dt, registry, input);
  DrawServices(dt, registry, input);
  DrawDevDebug(dt, registry, input);
  DrawToolBar(dt, registry, input);
  DrawShapes(dt, registry, input);
  DrawGameCamera(dt, registry, input);
}

void UiSystem::DrawHierarchy(double dt, Registry* registry, Input* input)
{
  ImGui::Begin("Hierarchy");

  UpdateInputActiveWindow(input, WindowType::Hierarchy);

  DevDebug& devDebug = registry->GetComponent<DevDebug>();
  devDebug.leftWindowWidth = ImGui::GetWindowWidth();

  // if (devDebug.changeFocusWindow == WindowType::Hierarchy) ImGui::SetWindowFocus();

  // TODO : This can be improved upon
  // For now just take anything that has a transform component attached to it

  // We will run into the same problem that AddComponent is facing right now
  // If we wanted a way to 'get all components that can be drawn'

  // Rather than that, we can define 'all components that can be drawn'
  // as primitives & 3d models.

  // TODO : The hierarchy is not drawn this way.

  std::vector<void*> cubeComponents = registry->GetComponents<Cube>();
  auto* cubes = static_cast<ComponentVector<Cube>*>(cubeComponents[0]);

  std::vector<void*> sphereComponents = registry->GetComponents<Sphere>();
  auto* spheres = static_cast<ComponentVector<Sphere>*>(sphereComponents[0]);

  std::vector<void*> modelComponents = registry->GetComponents<Model>();
  auto* models = static_cast<ComponentVector<Model>*>(modelComponents[0]);

  // Draw the cubes
  for (size_t i = 0; i < cubes->Size(); i++)
  {
    std::string modelName = "Cube : " + std::to_string(i);
    if (ImGui::CollapsingHeader(modelName.c_str()))
    {
    }
  }

  for (size_t i = 0; i < spheres->Size(); i++)
  {
    std::string modelName = "Sphere : " + std::to_string(i);
    if (ImGui::CollapsingHeader(modelName.c_str()))
    {
    }
  }

  for (size_t i = 0; i < models->Size(); i++)
  {
    std::string modelName = "Model : " + std::to_string(i);
    if (ImGui::CollapsingHeader(modelName.c_str()))
    {
    }
  }

  // ImGui::Text("This is the scene hierarchy");
  ImGui::End();
}

void UiSystem::DrawAssets(double dt, Registry* registry, Input* input)
{
  ImGui::Begin("Assets");
  UpdateInputActiveWindow(input, WindowType::Assets);

  DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::Assets) ImGui::SetWindowFocus();

  ImVec2 wsize = ImGui::GetWindowSize();

  devDebug.bottomWindowWidth = wsize.x;
  devDebug.bottomWindowHeight = wsize.y;

  ImGui::Text("This is the assets");
  ImGui::End();
}

void UiSystem::DrawScene(double dt, Registry* registry, Input* input)
{
  // Set no padding, as for the scene, there isn't really a need for padding
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Scene");
  // Pop it so that it applies to the entire window here.
  ImGui::PopStyleVar();

  ImGui::BeginChild("SceneRender");

  UpdateInputActiveWindow(input, WindowType::Scene);

  // Get the size of the current imgui window to draw in
  ImVec2 wsize = ImGui::GetWindowSize();

  SceneRender sceneRender = registry->GetComponent<SceneRender>();
  DevDebug& devDebug = registry->GetComponent<DevDebug>();

  // if (devDebug.changeFocusWindow == WindowType::Scene) ImGui::SetWindowFocus();

  devDebug.sceneWidth = wsize.x;
  devDebug.sceneHeight = wsize.y;

  // Flip V in the UV
  ImGui::Image((ImTextureID)sceneRender.textureID, wsize, ImVec2(0, 1), ImVec2(1, 0),
               ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));

  if (devDebug.activeEntity != 0)
  {
    ImGuizmo::BeginFrame();
    // ImGuizmo::EndFrame();

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
    float windowHeight = static_cast<float>(ImGui::GetWindowHeight());

    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    Transform& transform = *(registry->GetComponent<Transform>(devDebug.activeEntity));

    // TODO Create a utility method to compute this
    auto modelMatrix = GetModelMatrix(transform);
    ImGuizmo::Manipulate(glm::value_ptr(devDebug.view), glm::value_ptr(devDebug.projection),
                         devDebug.gizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(modelMatrix));

    if (ImGuizmo::IsUsing())
    {
      devDebug.onGizmo = true;
      // TODO : decompose the matrix model and find the transform, rotation, scale
      glm::vec3 position, scale;
      glm::quat rotation;

      // These are not needed, but are there to fulfill the decompose parameters
      glm::vec3 skew;
      glm::vec4 perspective;

      glm::decompose(modelMatrix, scale, rotation, position, skew, perspective);

      // Tell the compiler explicitly these are not used
      (void)skew;
      (void)perspective;

      // TODO : These rotation computations can just stay as quats for simplicity and not have to
      // be converted back into euler angles every time.
      glm::vec3 newRotation = glm::eulerAngles(rotation);
      glm::vec3 deltaRotation = newRotation - transform.rotation;

      transform.position = position;
      transform.rotation += deltaRotation;
      transform.scale = scale;
    }
    else
    {
      devDebug.onGizmo = false;
    }
  }
  ImGui::EndChild();

  ImGui::End();
}

void UiSystem::DrawGameCamera(double dt, Registry* registry, Input* input)
{
  // Set no padding, as for the scene, there isn't really a need for padding
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Game Camera");
  // Pop it so that it applies to the entire window here.
  ImGui::PopStyleVar();

  ImGui::BeginChild("SceneRender2");

  UpdateInputActiveWindow(input, WindowType::Scene);

  // Get the size of the current imgui window to draw in
  ImVec2 wsize = ImGui::GetWindowSize();

  SceneRender sceneRender = registry->GetComponent<SceneRender>();
  DevDebug& devDebug = registry->GetComponent<DevDebug>();

  // if (devDebug.changeFocusWindow == WindowType::Scene) ImGui::SetWindowFocus();

  devDebug.sceneWidth = wsize.x;
  devDebug.sceneHeight = wsize.y;

  // Flip V in the UV
  ImGui::Image((ImTextureID)sceneRender.textureID, wsize, ImVec2(0, 1), ImVec2(1, 0),
               ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));

  if (devDebug.activeEntity != 0)
  {
    ImGuizmo::BeginFrame();
    // ImGuizmo::EndFrame();

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
    float windowHeight = static_cast<float>(ImGui::GetWindowHeight());

    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    Transform& transform = *(registry->GetComponent<Transform>(devDebug.activeEntity));

    // TODO Create a utility method to compute this
    glm::mat4 matrixModel = glm::mat4(1.0f);
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    matrixModel = glm::translate(matrixModel, transform.position);
    matrixModel = glm::scale(matrixModel, transform.scale);

    // Rotation matrix
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
    rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

    matrixModel *= rotationMatrix;

    ImGuizmo::Manipulate(glm::value_ptr(devDebug.view), glm::value_ptr(devDebug.projection),
                         devDebug.gizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(matrixModel));

    if (ImGuizmo::IsUsing())
    {
      devDebug.onGizmo = true;
      // TODO : decompose the matrix model and find the transform, rotation, scale
      glm::vec3 position, scale;
      glm::quat rotation;

      // These are not needed, but are there to fulfill the decompose parameters
      glm::vec3 skew;
      glm::vec4 perspective;

      glm::decompose(matrixModel, scale, rotation, position, skew, perspective);

      // Tell the compiler explicitly these are not used
      (void)skew;
      (void)perspective;

      // TODO : These rotation computations can just stay as quats for simplicity and not have to
      // be converted back into euler angles every time.
      glm::vec3 newRotation = glm::eulerAngles(rotation);
      glm::vec3 deltaRotation = newRotation - transform.rotation;

      transform.position = position;
      transform.rotation += deltaRotation;
      transform.scale = scale;
    }
    else
    {
      devDebug.onGizmo = false;
    }
  }
  ImGui::EndChild();

  ImGui::End();
}

void UiSystem::DrawProject(double dt, Registry* registry, Input* input)
{
  ImGui::Begin("Project");

  UpdateInputActiveWindow(input, WindowType::Project);

  // DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::Scene) ImGui::SetWindowFocus();

  ImGui::Text("This is the project");
  ImGui::End();
}

void UiSystem::DrawConsole(double dt, Registry* registry, Input* input)
{
  ImGui::Begin("Console");

  UpdateInputActiveWindow(input, WindowType::Console);

  // DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::Console) ImGui::SetWindowFocus();

  ImGui::Text("This is the console");
  ImGui::End();
}

void UiSystem::DrawAnimation(double dt, Registry* registry, Input* input)
{
  ImGui::Begin("Animation");

  UpdateInputActiveWindow(input, WindowType::Animation);

  // DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::Animation) ImGui::SetWindowFocus();

  ImGui::Text("This is the animations");
  ImGui::End();
}

void UiSystem::DrawAnimator(double dt, Registry* registry, Input* input)
{
  ImGui::Begin("Animator");

  UpdateInputActiveWindow(input, WindowType::Animator);

  // DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::Animator) ImGui::SetWindowFocus();

  ImGui::Text("This is the animator");
  ImGui::End();
}

void UiSystem::DrawInspector(double dt, Registry* registry, Input* input)
{
  ImGui::Begin("Inspector");

  UpdateInputActiveWindow(input, WindowType::Inspector);

  DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::Inspector) ImGui::SetWindowFocus();

  if (devDebug.activeEntity == 0)
  {
    ImGui::End();
    return;
  }

  if (registry->EntityHasComponent<Cube>(devDebug.activeEntity))
  {
    Cube* cube = registry->GetComponent<Cube>(devDebug.activeEntity);
    ImGui::ColorEdit3("Color", &(cube->color.x));
  }
  else if (registry->EntityHasComponent<Sphere>(devDebug.activeEntity))
  {
    Sphere* sphere = registry->GetComponent<Sphere>(devDebug.activeEntity);
    ImGui::ColorEdit3("Color", &(sphere->color.x));
  }

  ImGui::End();
}

void UiSystem::DrawServices(double dt, Registry* registry, Input* input)
{
  ImGui::Begin("Services");

  UpdateInputActiveWindow(input, WindowType::Services);

  // DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::Services) ImGui::SetWindowFocus();

  ImGui::Text("This is the services");
  ImGui::End();
}

void UiSystem::DrawDevDebug(double dt, Registry* registry, Input* input)
{
  ImGui::Begin("DevDebug");

  UpdateInputActiveWindow(input, WindowType::DevDebug);

  DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::DevDebug) ImGui::SetWindowFocus();

  devDebug.rightWindowWidth = ImGui::GetWindowWidth();

  ImGui::Checkbox("Draw all with wireframe", &devDebug.drawWireframe);
  ImGui::Checkbox("Draw all colliders", &devDebug.drawColliders);

  ImGui::End();
}

void UiSystem::DrawShapes(double dt, Registry* registry, Input* input)
{
  ImGui::Begin("Default Assets");
  static const char* assets[3] = {"Sphere", "Cube", "Camera"};
  for (int n = 0; n < IM_ARRAYSIZE(assets); n++)
  {
    ImGui::PushID(n);
    if ((n % 3) != 0) ImGui::SameLine();
    ImGui::Button(assets[n], ImVec2(60, 60));

    // Our buttons are both drag sources and drag targets here!
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
      // Set payload to carry the index of our item (could be anything)
      ImGui::SetDragDropPayload("Default Assets", &n, sizeof(int));
      ImGui::Text(assets[n], assets[n]);
      ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
      if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Default Assets"))
      {
        IM_ASSERT(payload->DataSize == sizeof(int));
        int payload_n = *(const int*)payload->Data;
        assets[n] = assets[payload_n];
      }
      ImGui::EndDragDropTarget();
    }
    ImGui::PopID();
  }
  if (!test)
  {
    ImGui::Text("Hi");
  }
  ImGui::End();
}

void UiSystem::DrawToolBar(double dt, Registry* registry, Input* input)
{
  ImGuiWindowClass* window_class = new ImGuiWindowClass();
  window_class->DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(window_class);
  ImGui::Begin("ToolBar");
  ImVec2 wSize = ImGui::GetWindowSize();
  float buttonWidth = 60.0f;
  float totalPadding = 16;

  UpdateInputActiveWindow(input, WindowType::ToolBar);

  ImGui::SameLine((wSize.x - ((buttonWidth * 3) + totalPadding)) * 0.5);
  if (ImGui::Button("Play", ImVec2(buttonWidth, 0.0f)))
  {
  }
  ImGui::SameLine();
  if (ImGui::Button("Pause", ImVec2(buttonWidth, 0.0f)))
  {
  }
  ImGui::SameLine();
  if (ImGui::Button("Resume", ImVec2(buttonWidth, 0.0f)))
  {
  }

  // DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::Animator) ImGui::SetWindowFocus();

  ImGui::End();
}

void UiSystem::UpdateSceneWindow(Registry* registry, Input* input)
{
  DevDebug& devDebug = registry->GetComponent<DevDebug>();

  if (input->IsKeyDown('7'))
  {
    drawSceneOnly = !drawSceneOnly;
    input->SetKeyOff('7');
    if (drawSceneOnly)
    {
      devDebug.bottomWindowHeight = 0;
      devDebug.bottomWindowWidth = 0;
      devDebug.leftWindowHeight = 0;
      devDebug.leftWindowWidth = 0;
      devDebug.rightWindowHeight = 0;
      devDebug.rightWindowWidth = 0;
    }
  }
}

void UiSystem::UpdateGizmoType(Registry* registry, Input* input)
{
  DevDebug& devDebug = registry->GetComponent<DevDebug>();

  if (input->IsKeyDown('1'))
    devDebug.gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
  else if (input->IsKeyDown('2'))
    devDebug.gizmoOperation = ImGuizmo::OPERATION::ROTATE;
  else if (input->IsKeyDown('3'))
    devDebug.gizmoOperation = ImGuizmo::OPERATION::SCALE;
}

void UiSystem::UpdateInputActiveWindow(Input* input, WindowType windowType)
{
  if (ImGui::IsWindowFocused() && input->activeWindow != windowType)
  {
    input->activeWindow = windowType;
  }
}
