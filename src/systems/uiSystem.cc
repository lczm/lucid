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
  InitializeGUI();
}

void UiSystem::InitializeGUI() {
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
      ImGui::EndMainMenuBar();
    }
  }

  InitializeImGuiWindows();

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
  ImGui::DockBuilderFinish(dockSpaceID);
}

void UiSystem::InitializeImGuiWindows() {
  DrawHierarchy();
  DrawAssets();
  DrawScene();
  DrawProject();
  DrawConsole();
  DrawAnimation();
  DrawAnimator();
  DrawInspector();
  DrawServices();
}

void UiSystem::DrawHierarchy() {
  ImGui::Begin("Hierarchy");
  ImGui::Text("This is the scene hierarchy");
  ImGui::End();
}
void UiSystem::DrawAssets() {
  ImGui::Begin("Assets");
  ImGui::Text("This is the assets");
  ImGui::End();
}
void UiSystem::DrawScene() {
  ImGui::Begin("Scene");
  ImGui::End();
}
void UiSystem::DrawProject() {
  ImGui::Begin("Project");
  ImGui::Text("This is the project");
  ImGui::End();
}
void UiSystem::DrawConsole() {
  ImGui::Begin("Console");
  ImGui::Text("This is the console");
  ImGui::End();
}
void UiSystem::DrawAnimation() {
  ImGui::Begin("Animation");
  ImGui::Text("This is the animations");
  ImGui::End();
}
void UiSystem::DrawAnimator() {
  ImGui::Begin("Animator");
  ImGui::Text("This is the animator");
  ImGui::End();
}
void UiSystem::DrawInspector() {
  ImGui::Begin("Inspector");
  ImGui::Text("This is the inspector");
  ImGui::End();
}
void UiSystem::DrawServices() {
  ImGui::Begin("Services");
  ImGui::Text("This is the services");
  ImGui::End();
}
