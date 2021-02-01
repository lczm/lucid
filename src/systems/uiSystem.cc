#include "uiSystem.h"

UiSystem::UiSystem()
{
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
  io.IniFilename = NULL;                             // Disable imgui.ini
  io.ConfigWindowsMoveFromTitleBarOnly = true;
  (void)io;
}

UiSystem::~UiSystem() = default;

void UiSystem::Update(float dt, Registry* registry, Input* input)
{
  // Update the gizmo through input keys
  UpdateGizmoType(registry, input);

  InitializeGUI(dt, registry, input);
}

void UiSystem::InitializeGUI(float dt, Registry* registry, Input* input)
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
      InitializeImGuiWindows(dt, registry, input);
      PresetLayout(dockSpaceID);
    }

    ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), dockSpaceFlags);
    ImGui::End();

    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Open Project"))
        {
          NFD_Init();
          nfdchar_t* projectPath = NULL;
          nfdresult_t result = NFD_PickFolder(&projectPath, NULL);
          if (result == NFD_OKAY)
          {
            projectRoot = NewNode(projectPath, true);
            Workspace& workspace = registry->GetResource<Workspace>();
            workspace.projectRoot = projectRoot;
            AddFilesAndDirectoriesToRoot(projectRoot);
            SortFilesAndDirectories(projectRoot);
            NFD_Quit();
          }
          else if (result == NFD_CANCEL)
          {
          }
          else
          {
            std::cout << "NFD Error : " << NFD_GetError() << std::endl;
          }
        }
        if (ImGui::MenuItem("Create Project"))
        {
          drawSelectFolderPopup = true;
        }
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

      if (ImGui::MenuItem("Build"))
      {
        SerializeAllOut(registry);
      }

      if (input->IsKeyDown(GLFW_KEY_F3))
      {
        CompileUserGame();
      }

      if (input->IsKeyDown(GLFW_KEY_F4))
      {
        RunUserGame();
      }

      if (input->IsKeyDown(GLFW_KEY_F5))
      {
        SerializeAllOut(registry);
      }

      if (input->IsKeyDown(GLFW_KEY_F6))
      {
        SerializeAllIn(registry);
      }

      // TODO : Draw frametime
      // Draw FPS
      DevDebug& devDebug = registry->GetResource<DevDebug>();
      std::string fpsCount = "FPS : " + std::to_string(devDebug.fps);
      ImGui::Text(fpsCount.c_str());

      ImGui::EndMainMenuBar();
    }
  }

  if (drawSelectFolderPopup == true)
  {
    ImGui::OpenPopup("Select Folder");
    drawSelectFolderPopup = false;
  }

  if (ImGui::BeginPopupModal("Select Folder"))
  {
    // 260 size cause thats the maximum path length in windows
    static char directoryPath[260] = "";
    static char projectName[30] = "";
    static bool displayDirectoryError = false;
    static bool displayNameError = false;
    nfdchar_t* createProjectPath = NULL;
    ImGui::InputTextWithHint("##ProjectName", "Write a name for your project", projectName,
                             IM_ARRAYSIZE(projectName));
    ImGui::InputTextWithHint("##SelectDirectory", "Select a directory to create your project",
                             directoryPath, IM_ARRAYSIZE(directoryPath),
                             ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    if (ImGui::Button("Open"))
    {
      NFD_Init();
      nfdresult_t result = NFD_PickFolder(&createProjectPath, NULL);
      if (result == NFD_OKAY)
      {
        displayDirectoryError = false;
        strcpy_s(directoryPath, createProjectPath);
        NFD_Quit();
      }
      else if (result == NFD_CANCEL)
      {
      }
      else
      {
        std::cout << "NFD Error : " << NFD_GetError() << std::endl;
      }
    }
    if (ImGui::Button("Confirm"))
    {
      fs::path path = directoryPath;
      if (fs::exists(path))
      {
        // Check if user inputted a project name
        if (projectName[0] != '\0')
        {
          fs::path projectNameFolder = projectName;
          fs::path folder = "assets";
          fs::path assetPath = path / projectNameFolder / folder;
          fs::create_directories(assetPath);
          folder = ("systems");
          fs::path systemPath = path / projectNameFolder / folder;
          fs::create_directory(systemPath);
          strcpy_s(projectName, "");
          strcpy_s(directoryPath, "");
          displayNameError = false;
          displayDirectoryError = false;
          ImGui::CloseCurrentPopup();
        }
        else
        {
          displayNameError = true;
        }
      }
      else
      {
        displayDirectoryError = true;
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
      strcpy_s(projectName, "");
      strcpy_s(directoryPath, "");
      displayNameError = false;
      displayDirectoryError = false;
      ImGui::CloseCurrentPopup();
    }
    if (displayDirectoryError == true)
    {
      ImGui::TextWrapped("Directory not found, please try again.");
    }
    if (displayNameError == true)
    {
      ImGui::TextWrapped("Please input a project name.");
    }
    ImGui::EndPopup();
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
  ImGuiID dockTopID = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Up, 0.7f, NULL, &dockMainID);
  ImGuiID dockMiddleID =
      ImGui::DockBuilderSplitNode(dockTopID, ImGuiDir_Down, 0.95f, NULL, &dockTopID);
  ImGuiID dockBottomLeftID =
      ImGui::DockBuilderSplitNode(dockLeftID, ImGuiDir_Down, 0.3f, NULL, &dockLeftID);

  // Windows on left
  ImGui::DockBuilderDockWindow("Hierarchy", dockLeftID);
  ImGui::DockBuilderDockWindow("Game Configuration", dockLeftID);

  // Windows on bottom left
  ImGui::DockBuilderDockWindow("Project", dockBottomLeftID);
  ImGui::DockBuilderDockWindow("Console", dockBottomLeftID);

  // Windows on right
  ImGui::DockBuilderDockWindow("Inspector", dockRightID);
  ImGui::DockBuilderDockWindow("DevDebug", dockRightID);

  // Windows in the middle
  ImGui::DockBuilderDockWindow("Game Camera", dockMiddleID);
  ImGui::DockBuilderDockWindow("Scene", dockMiddleID);

  // Windows on the top
  ImGui::DockBuilderDockWindow("ToolBar", dockTopID);

  // Windows on the bottom
  ImGui::DockBuilderDockWindow("Assets", dockBottomID);
  ImGui::DockBuilderDockWindow("Default Assets", dockBottomID);
  ImGui::DockBuilderFinish(dockSpaceID);

  // Set initial window focus
  ImGui::SetWindowFocus("Scene");
}

void UiSystem::InitializeImGuiWindows(float dt, Registry* registry, Input* input)
{
  DrawHierarchy(dt, registry, input);
  DrawAssets(dt, registry, input);
  DrawGameConfiguration(dt, registry, input);
  DrawScene(dt, registry, input);
  DrawProject(dt, registry, input);
  DrawConsole(dt, registry, input);
  DrawInspector(dt, registry, input);
  DrawDevDebug(dt, registry, input);
  DrawToolBar(dt, registry, input);
  DrawDefaultAssets(dt, registry, input);
  DrawGameCamera(dt, registry, input);
}

void UiSystem::DrawHierarchy(float dt, Registry* registry, Input* input)
{
  ImGui::Begin("Hierarchy");

  // TODO : This can be improved upon
  // For now just take anything that has a transform component attached to it

  // We will run into the same problem that AddComponent is facing right now
  // If we wanted a way to 'get all components that can be drawn'

  // Rather than that, we can define 'all components that can be drawn'
  // as primitives & 3d models.

  // TODO : The hierarchy is not drawn this way.

  DevDebug& devDebug = registry->GetResource<DevDebug>();
  static Entity selected = -1;
  int i = 1;

  registry->GetComponentsIter<Cube>()->EachWithID([&](Entity id, Cube& cube) {
    std::string modelName = "Cube (" + std::to_string(i) + ")";
    i++;
    if (ImGui::Selectable(modelName.c_str(), selected == id))
    {
      selected = id;
      devDebug.activeEntity = id;
    }
  });

  i = 1;
  registry->GetComponentsIter<Sphere>()->EachWithID([&](Entity id, Sphere& cube) {
    std::string modelName = "Sphere (" + std::to_string(i) + ")";
    i++;
    if (ImGui::Selectable(modelName.c_str(), selected == id))
    {
      selected = id;
      devDebug.activeEntity = id;
    }
  });

  i = 1;
  registry->GetComponentsIter<Model>()->EachWithID([&](Entity id, Model& cube) {
    std::string modelName = "Model (" + std::to_string(i) + ")";
    i++;
    if (ImGui::Selectable(modelName.c_str(), selected == id))
    {
      selected = id;
      devDebug.activeEntity = id;
    }
  });

  UpdateInputActiveWindow(input, WindowType::Hierarchy);
  ImGui::End();
}

void UiSystem::DrawAssets(float dt, Registry* registry, Input* input)
{
  ImGui::Begin("Assets");

  UpdateInputActiveWindow(input, WindowType::Assets);
  ImGui::End();
}

void UiSystem::DrawScene(float dt, Registry* registry, Input* input)
{
  // Set no padding, as for the scene, there isn't really a need for padding
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Scene");
  // Pop it so that it applies to the entire window here.
  ImGui::PopStyleVar();

  ImGui::BeginChild("SceneRender");

  // Get the size of the current imgui window to draw in
  ImVec2 wsize = ImGui::GetWindowSize();

  SceneRender sceneRender = registry->GetResource<SceneRender>();
  DevDebug& devDebug = registry->GetResource<DevDebug>();
  WidgetLayout& widgetLayout = registry->GetResource<WidgetLayout>();

  //// if (devDebug.changeFocusWindow == WindowType::Scene) ImGui::SetWindowFocus();

  widgetLayout.sceneWidth = wsize.x;
  widgetLayout.sceneHeight = wsize.y;

  // Flip V in the UV
  ImGui::Image((ImTextureID)sceneRender.textureID, wsize, ImVec2(0, 1), ImVec2(1, 0),
               ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));

  HandleGizmoInput(registry, input);

  // UpdateWindowFocus(registry, WindowType::Inspector, "Inspector", input, WindowType::Scene);
  // UpdateWindowFocus(registry, WindowType::Scene, "Scene", input);

  UpdateWindowFocus(registry, WindowType::Inspector, "Inspector", input);

  UpdateInputActiveWindow(input, WindowType::Scene);
  ImGui::EndChild();

  // Drag n drop from default assets
  if (ImGui::BeginDragDropTarget())
  {
    // Check if drag and drop item is from default assets
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Default Assets"))
    {
      // Add assets to scene here
      IM_ASSERT(payload->DataSize == sizeof(int));
      DefaultAssetsType payloadN = *(const DefaultAssetsType*)payload->Data;
      switch (payloadN)
      {
        case DefaultAssetsType::Cube:
        {
          Entity cubeID = registry->GetAvailableEntityId();
          registry->CreateEntity<Cube, Transform>(cubeID);
          Camera& camera = registry->GetResource<Camera>();
          registry->GetComponent<Transform>(cubeID).position = camera.GetPositionInWorld();
        }
        break;
        case DefaultAssetsType::Sphere:
        {
          Entity sphereID = registry->GetAvailableEntityId();
          registry->CreateEntity<Sphere, Transform>(sphereID);
          Camera& camera = registry->GetResource<Camera>();
          registry->GetComponent<Transform>(sphereID).position = camera.GetPositionInWorld();
        }
        break;
        case DefaultAssetsType::Line:
          break;
      }

      devDebug.changeFocusWindow = WindowType::Scene;
      UpdateWindowFocus(registry, WindowType::DefaultAssets, "Scene", input);
    }
    // Check if payload is from project window
    else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Project"))
    {
      IM_ASSERT(payload->DataSize == sizeof(Node));
      Node payloadN = *(const Node*)payload->Data;
      // placeholders to confirm that dragging and dropping models works
      Entity assetId = registry->GetAvailableEntityId();
      Camera& camera = registry->GetResource<Camera>();
      registry->CreateEntity<Model, Transform, ColliderPolygon>(assetId);
      registry->AddComponentData<Model>(assetId, Model(payloadN.path.string()));
      registry->GetComponent<Model>(assetId).toAnimate = false;
      registry->GetComponent<Transform>(assetId).position = camera.GetPositionInWorld();
      registry->GetComponent<Transform>(assetId).scale /= 150.0f;
    }
    ImGui::EndDragDropTarget();
  }

  ImVec2 sceneWindowSize = ImGui::GetWindowSize();
  ImVec2 scenePos = ImGui::GetWindowPos();

  widgetLayout.menuBarHeight = 19;
  widgetLayout.topWindowHeight = scenePos.y;
  widgetLayout.leftWindowWidth = scenePos.x;
  widgetLayout.bottomWindowHeight = SCREEN_HEIGHT - (scenePos.y + sceneWindowSize.y);
  widgetLayout.rightWindowWidth = SCREEN_WIDTH - (scenePos.x + sceneWindowSize.x);

  ImGui::End();
}

void UiSystem::DrawGameCamera(float dt, Registry* registry, Input* input)
{
  // Set no padding, as for the scene, there isn't really a need for padding
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Game Camera");
  // Pop it so that it applies to the entire window here.
  ImGui::PopStyleVar();

  ImGui::BeginChild("SceneRender");

  // Get the size of the current imgui window to draw in
  ImVec2 wsize = ImGui::GetWindowSize();

  SceneRender sceneRender = registry->GetResource<SceneRender>();
  DevDebug& devDebug = registry->GetResource<DevDebug>();

  // This should draw from sceneRender fbo that is being rendered through the gameScene camera
  // Flip V in the UV
  // ImGui::Image((ImTextureID)sceneRender.textureID, wsize, ImVec2(0, 1), ImVec2(1, 0),
  //              ImVec4(1, 1, 1, 1), ImVec4(0, 0, 0, 0));

  ImGui::EndChild();

  UpdateInputActiveWindow(input, WindowType::GameCamera);
  ImGui::End();
}

void UiSystem::DrawProject(float dt, Registry* registry, Input* input)
{
  ImGui::Begin("Project");
  // DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::Scene) ImGui::SetWindowFocus();

  UpdateInputActiveWindow(input, WindowType::Project);

  // Check if a folder has been selected
  if (!fs::exists(projectRoot->path))
  {
    ImGui::End();
    return;
  }

  // Tentative button will probably change to right click menu soon
  if (ImGui::Button("Refresh"))
  {
    projectRoot = NewNode(projectRoot->path, true);
    AddFilesAndDirectoriesToRoot(projectRoot);
    SortFilesAndDirectories(projectRoot);
  }
  DrawFileTree(projectRoot);

  ImGui::End();
}

void UiSystem::DrawConsole(float dt, Registry* registry, Input* input)
{
  ImGui::Begin("Console");

  // DevDebug& devDebug = registry->GetComponent<DevDebug>();
  // if (devDebug.changeFocusWindow == WindowType::Console) ImGui::SetWindowFocus();

  ImGui::Text("This is the console");
  UpdateInputActiveWindow(input, WindowType::Console);
  ImGui::End();
}

void UiSystem::DrawInspector(float dt, Registry* registry, Input* input)
{
  ImGui::Begin("Inspector");

  DevDebug& devDebug = registry->GetResource<DevDebug>();
  ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
  static std::string currentItem = "";

  UpdateInputActiveWindow(input, WindowType::Inspector);
  if (devDebug.activeEntity == 0)
  {
    ImGui::End();
    return;
  }

  DrawInspectorCubeComponent(registry, devDebug);
  DrawInspectorSphereComponent(registry, devDebug);
  DrawInspectorTransformComponent(registry, devDebug);
  DrawInspectorRigidBodyComponent(registry, devDebug, input);

  if (ImGui::CollapsingHeader("Add Component", treeNodeFlags))
  {
    if (ImGui::BeginCombo("##Add Component", currentItem.c_str()))
    {
      ImGui::EndCombo();
    }
    ImGui::SameLine(0);
    if (ImGui::Button("Add"))
    {
    }
    ImGui::Separator();
  }

  ImGui::End();
}

void UiSystem::DrawDevDebug(float dt, Registry* registry, Input* input)
{
  ImGui::Begin("DevDebug");

  DevDebug& devDebug = registry->GetResource<DevDebug>();
  // WidgetLayout& widgetLayout = registry->GetComponent<WidgetLayout>();
  RendererStats& rendererStats = registry->GetResource<RendererStats>();

  // widgetLayout.rightWindowWidth = ImGui::GetWindowWidth();
  // if (devDebug.changeFocusWindow == WindowType::DevDebug) ImGui::SetWindowFocus();

  ImGui::Checkbox("Draw all with wireframe", &devDebug.drawWireframe);
  ImGui::Checkbox("Draw all colliders", &devDebug.drawColliders);

  std::string callsPerFrame = "Calls per frame : " + std::to_string(rendererStats.calls);
  ImGui::Text(callsPerFrame.c_str());

  std::string verticesPerFrame = "Vertices per frame : " + std::to_string(rendererStats.vertices);
  ImGui::Text(verticesPerFrame.c_str());

  std::string indicesPerFrame = "Indices per frame : " + std::to_string(rendererStats.indices);
  ImGui::Text(indicesPerFrame.c_str());

  UpdateInputActiveWindow(input, WindowType::DevDebug);
  ImGui::End();
}

void UiSystem::DrawDefaultAssets(float dt, Registry* registry, Input* input)
{
  ImGui::Begin("Default Assets");
  for (size_t n = 0; n < defaultAssets.size(); n++)
  {
    ImGui::PushID(n);
    if ((n % 10) != 0) ImGui::SameLine();
    ImGui::Button(defaultAssets.at(n).second.c_str(), ImVec2(60, 60));

    // Set buttons as drag and drop source
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
      // Set payload to carry the index of the asset
      // but will check for it using the DefaultAssetType enum which im not sure
      // if i shouldnt be doing it
      ImGui::SetDragDropPayload("Default Assets", &n, sizeof(int));
      ImGui::Text(defaultAssets.at(n).second.c_str());
      ImGui::EndDragDropSource();
    }
    ImGui::PopID();
  }
  UpdateInputActiveWindow(input, WindowType::DefaultAssets);
  ImGui::End();
}

void UiSystem::DrawToolBar(float dt, Registry* registry, Input* input)
{
  ImGuiWindowClass windowClass;
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  DevDebug& devDebug = registry->GetResource<DevDebug>();
  float buttonWidth = 60.0f;
  float totalPadding = 16;

  windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&windowClass);
  ImGui::Begin("ToolBar", (bool*)0, windowFlags);
  ImVec2 wSize = ImGui::GetWindowSize();

  GameEngineState& gameEngineState = registry->GetResource<GameEngineState>();

  if (devDebug.activeEntity != 0)
  {
    std::string gizmoType;
    std::string gizmoTypeString;
    const char* gizmoTypeChar;
    switch (devDebug.gizmoOperation)
    {
      case ImGuizmo::OPERATION::ROTATE:
        gizmoType = "Rotate";
        break;
      case ImGuizmo::OPERATION::TRANSLATE:
        gizmoType = "Translate";
        break;
      case ImGuizmo::OPERATION::SCALE:
        gizmoType = "Scale";
        break;
      case ImGuizmo::OPERATION::BOUNDS:
        gizmoType = "Bounds";
        break;
    }

    gizmoTypeString = "Gizmo Type: " + gizmoType;
    gizmoTypeChar = gizmoTypeString.c_str();
    ImGui::Text(gizmoTypeChar);
  }
  ImGui::SameLine(static_cast<float>((wSize.x - ((buttonWidth * 3) + totalPadding)) * 0.5));
  if (ImGui::Button("Play", ImVec2(buttonWidth, 0.0f)))
  {
    gameEngineState.gameState = GameState::PLAYING;
  }

  ImGui::SameLine();
  if (ImGui::Button("Pause", ImVec2(buttonWidth, 0.0f)))
  {
    gameEngineState.gameState = GameState::PAUSED;
  }

  ImGui::SameLine();
  if (ImGui::Button("Restart", ImVec2(buttonWidth, 0.0f)))
  {
  }

  ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 60);
  if (ImGui::Button("Reset", ImVec2(buttonWidth, 0.0f)))
  {
    Camera& camera = registry->GetResource<Camera>();
    camera.position = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    camera.TranslateInWorld({0.0f, 1.0f, 20.0f});
  }

  UpdateInputActiveWindow(input, WindowType::ToolBar);
  ImGui::End();
}

void UiSystem::DrawGameConfiguration(float dt, Registry* registry, Input* input)
{
  ImGui::Begin("Game Configuration");
  // if (devDebug.changeFocusWindow == WindowType::Assets) ImGui::SetWindowFocus();

  ImGui::Text("This is the game configuration");
  UpdateInputActiveWindow(input, WindowType::GameConfiguration);
  ImGui::End();
}

// Need to call this in a loop
void UiSystem::DrawProgressBar(float fraction, std::string message)
{
  float barWidth = 400.0f;
  ImGuiIO& io = ImGui::GetIO();
  const char* castedMessage = message.c_str();
  ImGui::SetNextWindowSize(ImVec2(barWidth, 0), ImGuiCond_Appearing);
  ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2, io.DisplaySize.y / 2), ImGuiCond_Appearing,
                          ImVec2(0.5f, 0.5f));
  ImGui::Begin("Progress Bar", (bool*)0, ImGuiWindowFlags_NoTitleBar);
  ImGui::TextWrapped("%s %c", castedMessage, "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
  ImGui::ProgressBar(fraction);
  ImGui::End();
}

void UiSystem::DrawInspectorCubeComponent(Registry* registry, DevDebug& devDebug,
                                          ImGuiTreeNodeFlags treeNodeFlags)
{
  if (registry->EntityHasComponent<Cube>(devDebug.activeEntity))
  {
    if (ImGui::CollapsingHeader("Color", treeNodeFlags))
    {
      if (ImGui::BeginPopupContextItem("Cube color context menu"))
      {
        if (ImGui::MenuItem("Remove Component"))
        {
          registry->RemoveComponent<RigidBody>(devDebug.activeEntity);
        }

        ImGui::EndPopup();
      }
      Cube& cube = registry->GetComponent<Cube>(devDebug.activeEntity);
      ImGui::ColorEdit3("Color", &(cube.color.x));
    }
    ImGui::Separator();
  }
}

void UiSystem::DrawInspectorSphereComponent(Registry* registry, DevDebug& devDebug,
                                            ImGuiTreeNodeFlags treeNodeFlags)
{
  if (registry->EntityHasComponent<Sphere>(devDebug.activeEntity))
  {
    if (ImGui::CollapsingHeader("Color", treeNodeFlags))
    {
      if (ImGui::BeginPopupContextItem("Sphere color context menu"))
      {
        if (ImGui::MenuItem("Remove Component"))
        {
          registry->RemoveComponent<RigidBody>(devDebug.activeEntity);
        }

        ImGui::EndPopup();
      }
      Sphere& sphere = registry->GetComponent<Sphere>(devDebug.activeEntity);
      ImGui::ColorEdit3("Color", &(sphere.color.x));
    }
    ImGui::Separator();
  }
}

void UiSystem::DrawInspectorTransformComponent(Registry* registry, DevDebug& devDebug,
                                               ImGuiTreeNodeFlags treeNodeFlags)
{
  if (registry->EntityHasComponent<Transform>(devDebug.activeEntity))
  {
    if (ImGui::CollapsingHeader("Transform", treeNodeFlags))
    {
      // right click to remove component
      if (ImGui::BeginPopupContextItem("Transform context menu"))
      {
        if (ImGui::MenuItem("Remove Component"))
        {
          registry->RemoveComponent<RigidBody>(devDebug.activeEntity);
        }

        ImGui::EndPopup();
      }
      Transform& transform = registry->GetComponent<Transform>(devDebug.activeEntity);
      ImGui::InputFloat("x position", &(transform.position.x), 0.25f, 1.0f);
      ImGui::InputFloat("y position", &(transform.position.y), 0.25f, 1.0f);
      ImGui::InputFloat("z position", &(transform.position.z), 0.25f, 1.0f);

      ImGui::InputFloat("x rotation", &(transform.rotation.x), 0.25f, 1.0f);
      ImGui::InputFloat("y rotation", &(transform.rotation.y), 0.25f, 1.0f);
      ImGui::InputFloat("z rotation", &(transform.rotation.z), 0.25f, 1.0f);

      ImGui::InputFloat("x scale", &(transform.scale.x), 0.25f, 1.0f);
      ImGui::InputFloat("y scale", &(transform.scale.y), 0.25f, 1.0f);
      ImGui::InputFloat("z scale", &(transform.scale.z), 0.25f, 1.0f);
    }
    ImGui::Separator();
  }
}

void UiSystem::DrawInspectorRigidBodyComponent(Registry* registry, DevDebug& devDebug, Input* input,
                                               ImGuiTreeNodeFlags treeNodeFlags)
{
  if (registry->EntityHasComponent<RigidBody>(devDebug.activeEntity))
  {
    if (ImGui::CollapsingHeader("Rigid Body", treeNodeFlags))
    {
      // right click to remove component
      if (ImGui::BeginPopupContextItem("rigidbody context menu"))
      {
        if (ImGui::MenuItem("Remove Component"))
        {
          registry->RemoveComponent<RigidBody>(devDebug.activeEntity);
        }

        ImGui::EndPopup();
      }
      RigidBody& rigidBody = registry->GetComponent<RigidBody>(devDebug.activeEntity);
      ImGui::InputFloat("x velocity", &(rigidBody.velocity.x), 0.25f, 1.0f);
      ImGui::InputFloat("y velocity", &(rigidBody.velocity.y), 0.25f, 1.0f);
      ImGui::InputFloat("z velocity", &(rigidBody.velocity.z), 0.25f, 1.0f);
      ImGui::Checkbox("Apply gravity", &(rigidBody.applyGravity));
    }
    ImGui::Separator();
  }
}

void UiSystem::HandleGizmoInput(Registry* registry, Input* input)
{
  DevDebug& devDebug = registry->GetResource<DevDebug>();

  if (devDebug.activeEntity != 0)
  {
    ImGuizmo::BeginFrame();

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();

    float windowWidth = static_cast<float>(ImGui::GetWindowWidth());
    float windowHeight = static_cast<float>(ImGui::GetWindowHeight());

    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    Transform& transform = registry->GetComponent<Transform>(devDebug.activeEntity);

    // TODO Create a utility method to compute this
    auto modelMatrix = GetModelMatrix(transform);
    Camera& camera = registry->GetResource<Camera>();
    ImGuizmo::Manipulate(glm::value_ptr(camera.GetView()), glm::value_ptr(camera.GetProjection()),
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

      transform.position = position;
      transform.rotation = rotation;
      transform.scale = scale;
    }
    else
    {
      devDebug.onGizmo = false;
    }
  }
}

void UiSystem::UpdateSceneWindow(Registry* registry, Input* input)
{
  if (input->IsKeyDown('7'))
  {
    drawSceneOnly = !drawSceneOnly;
    input->SetKeyOff('7');
  }
}

void UiSystem::UpdateGizmoType(Registry* registry, Input* input)
{
  DevDebug& devDebug = registry->GetResource<DevDebug>();

  if (input->IsKeyDown('1'))
    devDebug.gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
  else if (input->IsKeyDown('2'))
    devDebug.gizmoOperation = ImGuizmo::OPERATION::ROTATE;
  else if (input->IsKeyDown('3'))
    devDebug.gizmoOperation = ImGuizmo::OPERATION::SCALE;
}

void UiSystem::UpdateInputActiveWindow(Input* input, WindowType windowType)
{
  if (ImGui::IsWindowFocused() && ImGui::IsWindowHovered() && input->activeWindow != windowType)
  {
    input->activeWindow = windowType;
  }
}

void UiSystem::UpdateWindowFocus(Registry* registry, WindowType windowType, std::string focusWindow,
                                 Input* input, WindowType changeFocusWindow)
{
  DevDebug& devDebug = registry->GetResource<DevDebug>();

  if (devDebug.changeFocusWindow == windowType)
  {
    const char* charFocusWindow = focusWindow.c_str();
    ImGui::SetWindowFocus(charFocusWindow);
    input->activeWindow = windowType;
    devDebug.changeFocusWindow = changeFocusWindow;
  }
}