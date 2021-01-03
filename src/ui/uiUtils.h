#pragma once
#include <vector>
#include <iostream>
#include <filesystem>
#include "imgui.h"
#include "nfd.h"

struct Node
{
  std::filesystem::path path;
  bool isDirectory;

  std::vector<Node*> child;
};

static Node* NewNode(std::filesystem::path path, bool isDirectory)
{
  Node* temp = new Node;
  temp->path = path;
  temp->isDirectory = isDirectory;
  return temp;
}

// Recursively populates the project root with files and directories
static void AddFilesAndDirectoriesToRoot(Node*& node)
{
  if (std::filesystem::is_empty(node->path))
  {
    return;
  }

  for (const auto& entry : std::filesystem::directory_iterator(node->path))
  {
    if (!std::filesystem::is_directory(entry))
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
  for (size_t i = 0; i < node->child.size(); i++)
  {
    ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow |
                                       ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                       ImGuiTreeNodeFlags_SpanAvailWidth;
    if (node->child.at(i)->isDirectory == false)
    {
      treeNodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
      std::string stringFilePath = node->child.at(i)->path.filename().string();
      const char* charFilePath = stringFilePath.c_str();
      ImGui::TreeNodeEx(charFilePath, treeNodeFlags);
    }
    else
    {
      std::string stringFilePath = node->child.at(i)->path.filename().string();
      const char* charFilePath = stringFilePath.c_str();
      bool nodeOpen = ImGui::TreeNodeEx(charFilePath, treeNodeFlags);
      if (nodeOpen)
      {
        DrawFileTree(node->child.at(i));
        ImGui::TreePop();
      }
    }
  }
}
