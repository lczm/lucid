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

static Node* NewNode(std::filesystem::path path)
{
  Node* temp = new Node;
  temp->path = path;
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
    if (entry.is_regular_file())
    {
      node->isDirectory = false;
      node->child.push_back(NewNode(entry));
    }
    else
    {
      node->isDirectory = true;
      node->child.push_back(NewNode(entry));
      Node* directoryNode = node->child.back();
      AddFilesAndDirectoriesToRoot(directoryNode);
    }
  }
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
