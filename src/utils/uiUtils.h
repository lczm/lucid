#pragma once
#include <vector>
#include <iostream>
#include <filesystem>
#include "imgui.h"
#include "nfd.h"

namespace fs = std::filesystem;

struct Node
{
  fs::path path;
  bool isDirectory;

  std::vector<Node*> child;
};

static Node* NewNode(fs::path path, bool isDirectory)
{
  Node* temp = new Node;
  temp->path = path;
  temp->isDirectory = isDirectory;
  return temp;
}

// Recursively populates the project root with files and directories
static void AddFilesAndDirectoriesToRoot(Node*& node)
{
  if (fs::is_empty(node->path))
  {
    return;
  }

  for (const auto& entry : fs::directory_iterator(node->path))
  {
    if (!fs::is_directory(entry))
    {
      Node* temp = NewNode(entry, false);
      node->child.push_back(temp);
    }
    else
    {
      Node* temp = NewNode(entry, true);
      node->child.push_back(temp);
      Node* directoryNode = node->child.back();
      AddFilesAndDirectoriesToRoot(directoryNode);
    }
  }
}

static std::vector<std::string> GetFilesAndDirectoriesFromPath(fs::path path)
{
  std::vector<std::string> filesAndDirectories;

  if (fs::is_empty(path))
  {
    return filesAndDirectories;
  }

  for (const auto& entry : fs::directory_iterator(path))
  {
    if (!fs::is_directory(entry))
    {
      filesAndDirectories.push_back(entry.path().string());
    }
    else
    {
      filesAndDirectories.push_back(entry.path().string());
      std::vector<std::string> directoryFiles = GetFilesAndDirectoriesFromPath(entry.path());
      filesAndDirectories.insert(filesAndDirectories.end(), directoryFiles.begin(),
                                 directoryFiles.end());
    }
  }

  return filesAndDirectories;
}

static std::vector<std::string> GetFilesAndDirectoriesFromPath(std::string path)
{
  std::vector<std::string> filesAndDirectories;

  if (fs::is_empty(path))
  {
    return filesAndDirectories;
  }

  for (const auto& entry : fs::directory_iterator(path))
  {
    if (!fs::is_directory(entry))
    {
      filesAndDirectories.push_back(entry.path().string());
    }
    else
    {
      filesAndDirectories.push_back(entry.path().string());
      std::vector<std::string> directoryFiles =
          GetFilesAndDirectoriesFromPath(entry.path().string());
      filesAndDirectories.insert(filesAndDirectories.end(), directoryFiles.begin(),
                                 directoryFiles.end());
    }
  }

  return filesAndDirectories;
}

static void SortFilesAndDirectories(Node*& node)
{
  std::vector<Node*> directories;
  std::vector<Node*> files;
  for (size_t i = 0; i < node->child.size(); i++)
  {
    if (node->child.at(i)->isDirectory == false)
    {
      files.push_back(node->child.at(i));
    }
    else
    {
      directories.push_back(node->child.at(i));
      SortFilesAndDirectories(node->child.at(i));
    }
  }

  // Append directories to be infront of files
  directories.insert(directories.end(), files.begin(), files.end());
  node->child = directories;
}

static void DrawFileTree(Node* node)
{
  Node* nodeClicked = NULL;
  static Node* selectionMask = NULL;
  for (size_t i = 0; i < node->child.size(); i++)
  {
    ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow |
                                       ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                       ImGuiTreeNodeFlags_SpanAvailWidth;
    const bool isSelected = (selectionMask == node->child.at(i));
    if (isSelected) treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
    if (node->child.at(i)->isDirectory == false)
    {
      treeNodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
      std::string stringFilePath = node->child.at(i)->path.filename().string();
      const char* charFilePath = stringFilePath.c_str();
      ImGui::TreeNodeEx(charFilePath, treeNodeFlags);
      if (isSelected)
      {
        Node* currentFile = node->child.at(i);
        if (currentFile->path.extension() == ".gltf" ||
            currentFile->path.extension() == ".fbx" ||
            currentFile->path.extension() == ".obj")
        {
          // Filename without extension
          std::string stringCurrentFileName = currentFile->path.stem().string();
          const char* charCurrentFileName = stringCurrentFileName.c_str();

          // Set tree node as drag and drop source
          if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
          {
            ImGui::SetDragDropPayload("Project", currentFile, sizeof(Node));
            ImGui::EndDragDropSource();
          }
        }
      }
      if (ImGui::IsItemClicked()) nodeClicked = node->child.at(i);
    }
    else
    {
      std::string stringFilePath = node->child.at(i)->path.filename().string();
      const char* charFilePath = stringFilePath.c_str();
      bool nodeOpen = ImGui::TreeNodeEx(charFilePath, treeNodeFlags);
      if (isSelected)
      {
        ImGui::Begin("Assets");
        Node* selectedDirectory = node->child.at(i);
        // for each of the files in the selected directory
        for (size_t j = 0; j < selectedDirectory->child.size(); j++)
        {
          Node* currentFile = selectedDirectory->child.at(j);
          if (currentFile->path.extension() == ".gltf")
          {
            std::string stringCurrentFileName = currentFile->path.stem().string();
            const char* charCurrentFileName = stringCurrentFileName.c_str();
            ImGui::PushID(j);
            if ((j % 10) != 0) ImGui::SameLine();
            ImGui::Button(charCurrentFileName, ImVec2(60, 60));

            // Set buttons as drag and drop source
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
              ImGui::SetDragDropPayload("Project", currentFile, sizeof(Node));
              ImGui::Text(charCurrentFileName);
              ImGui::EndDragDropSource();
            }
            ImGui::PopID();
          }
        }
        ImGui::End();
      }
      if (ImGui::IsItemClicked()) nodeClicked = node->child.at(i);
      if (nodeOpen)
      {
        DrawFileTree(node->child.at(i));
        ImGui::TreePop();
      }
    }
  }

  if (nodeClicked != NULL)
  {
    selectionMask = nodeClicked;
  }
}
