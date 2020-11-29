#include "uiSystem.h"

UiSystem::UiSystem() {
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
  io.IniFilename = NULL;                             // Disable imgui.ini
  io.ConfigWindowsMoveFromTitleBarOnly |= ImGuiWindowFlags_NoMove;
  (void)io;
}

UiSystem::~UiSystem() = default;

void UiSystem::Update(double dt, Registry* registry, Input* input) {
  InitializeGUI(dt, registry, input);
}

void UiSystem::InitializeGUI(double dt, Registry* registry, Input* input) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
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
    if (!ImGui::DockBuilderGetNode(dockSpaceID)) {
      PresetLayout(dockSpaceID);
    }

    ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), dockSpaceFlags);
    ImGui::End();

    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit")) {
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Help")) {
        ImGui::Text("WASD - Move the camera");
        ImGui::Text("Arrow Left/Right/Up/Down - Rotate Left/Right/Up/Down");
        ImGui::Text("J/L - [Demo] pong player movement");
        ImGui::Text("Mouse left - Pan Camera / [Debug] Shoot rays");
        ImGui::Text("Mouse right - Rotate Camera");
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

  InitializeImGuiWindows(dt, registry, input);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UiSystem::PresetLayout(ImGuiID dockSpaceID) {
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
  ImGuiID dockTopID = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Up, 0.2f, NULL, &dockMainID);
  ImGuiID dockBottomLeftID =
      ImGui::DockBuilderSplitNode(dockLeftID, ImGuiDir_Down, 0.3f, NULL, &dockLeftID);

  ImGui::DockBuilderDockWindow("Hierarchy", dockLeftID);
  ImGui::DockBuilderDockWindow("Project", dockBottomLeftID);
  ImGui::DockBuilderDockWindow("Console", dockBottomLeftID);
  ImGui::DockBuilderDockWindow("Animation", dockBottomLeftID);
  ImGui::DockBuilderDockWindow("Animator", dockBottomLeftID);
  ImGui::DockBuilderDockWindow("Inspector", dockRightID);
  ImGui::DockBuilderDockWindow("Services", dockRightID);
  ImGui::DockBuilderDockWindow("Assets", dockBottomID);
  ImGui::DockBuilderDockWindow("Scene", dockTopID);
  ImGui::DockBuilderDockWindow("DevDebug", dockRightID);
  ImGui::DockBuilderFinish(dockSpaceID);
}

void UiSystem::InitializeImGuiWindows(double dt, Registry* registry, Input* input) {
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
}

void UiSystem::DrawHierarchy(double dt, Registry* registry, Input* input) {
  ImGui::Begin("Hierarchy");

  UpdateInputActiveWindow(input, WindowType::Hierarchy);

  DevDebug& devDebug = registry->GetComponent<DevDebug>();
  devDebug.leftWindowWidth = ImGui::GetWindowWidth();

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
  for (size_t i = 0; i < cubes->Size(); i++) {
    std::string modelName = "Cube : " + std::to_string(i);
    if (ImGui::CollapsingHeader(modelName.c_str())) {
    }
  }

  for (size_t i = 0; i < spheres->Size(); i++) {
    std::string modelName = "Sphere : " + std::to_string(i);
    if (ImGui::CollapsingHeader(modelName.c_str())) {
    }
  }

  for (size_t i = 0; i < models->Size(); i++) {
    std::string modelName = "Model : " + std::to_string(i);
    if (ImGui::CollapsingHeader(modelName.c_str())) {
    }
  }

  // ImGui::Text("This is the scene hierarchy");
  ImGui::End();
}

void UiSystem::DrawAssets(double dt, Registry* registry, Input* input) {
  ImGui::Begin("Assets");

  UpdateInputActiveWindow(input, WindowType::Assets);

  ImGui::Text("This is the assets");
  ImGui::End();
}

void UiSystem::DrawScene(double dt, Registry* registry, Input* input) {
  ImGui::Begin("Scene");

  ImGui::BeginChild("SceneRender");

  UpdateInputActiveWindow(input, WindowType::Scene);

  // Get the size of the current imgui window to draw in
  ImVec2 wsize = ImGui::GetWindowSize();

  SceneRender sceneRender = registry->GetComponent<SceneRender>();

  // Flip V in the UV
  ImGui::Image((ImTextureID)sceneRender.textureID, wsize, ImVec2(0, 1), ImVec2(1, 0));

  ImGui::EndChild();

  ImGui::End();
}

void UiSystem::DrawProject(double dt, Registry* registry, Input* input) {
  ImGui::Begin("Project");

  UpdateInputActiveWindow(input, WindowType::Project);

  ImGui::Text("This is the project");
  ImGui::End();
}

void UiSystem::DrawConsole(double dt, Registry* registry, Input* input) {
  ImGui::Begin("Console");

  UpdateInputActiveWindow(input, WindowType::Console);

  ImGui::Text("This is the console");
  ImGui::End();
}

void UiSystem::DrawAnimation(double dt, Registry* registry, Input* input) {
  ImGui::Begin("Animation");

  UpdateInputActiveWindow(input, WindowType::Animation);

  ImGui::Text("This is the animations");
  ImGui::End();
}

void UiSystem::DrawAnimator(double dt, Registry* registry, Input* input) {
  ImGui::Begin("Animator");

  UpdateInputActiveWindow(input, WindowType::Animator);

  ImGui::Text("This is the animator");
  ImGui::End();
}

void UiSystem::DrawInspector(double dt, Registry* registry, Input* input) {
  ImGui::Begin("Inspector");

  UpdateInputActiveWindow(input, WindowType::Inspector);

  // Temporary : this will be useful when mouse picking works
  // DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // ImGui::ColorEdit3("Primitive", (float*)&devDebug.rgb);

  ImGui::End();
}

void UiSystem::DrawServices(double dt, Registry* registry, Input* input) {
  ImGui::Begin("Services");

  UpdateInputActiveWindow(input, WindowType::Services);

  ImGui::Text("This is the services");
  ImGui::End();
}

void UiSystem::DrawDevDebug(double dt, Registry* registry, Input* input) {
  ImGui::Begin("DevDebug");

  UpdateInputActiveWindow(input, WindowType::DevDebug);

  DevDebug& devDebug = registry->GetComponent<DevDebug>();
  devDebug.rightWindowWidth = ImGui::GetWindowWidth();

  ImGui::Checkbox("Draw all with wireframe", &devDebug.drawWireframe);
  ImGui::Checkbox("Draw all colliders", &devDebug.drawColliders);

  ImGui::End();
}

void UiSystem::UpdateInputActiveWindow(Input* input, WindowType windowType) {
  if (ImGui::IsWindowFocused() && input->activeWindow != windowType) {
    input->activeWindow = windowType;
  }
}
