#pragma once

#include <string>
#include <stdlib.h>

#include "ecs.h"
#include "engineComponents.h"

namespace fs = std::filesystem;

static std::string ConvertFsToNativePaths(std::string path)
{
  std::string convertedPath = "";
  for (auto c : path)
  {
    if (c == '\\')
    {
      convertedPath += "/";
    }
    else
    {
      convertedPath += c;
    }
  }

  return convertedPath;
}

static std::string RemoveLastObjectFromPath(std::string path, char delimiter)
{
  std::vector<uint32_t> indexs;

  for (uint32_t i = 0; i < path.size(); i++)
  {
    if (path[i] == delimiter)
    {
      indexs.push_back(i);
    }
  }

  if (indexs.size() == 0)
  {
    return path;
  }

  uint32_t count = 0;
  std::string convertedPath = "";

  for (auto c : path)
  {
    if (c == delimiter)
    {
      count++;

      if (count == indexs.size())
      {
        return convertedPath;
      }
    }

    convertedPath += c;
  }

  return path;
}

static void CompileUserGameDebug(Registry* registry)
{
  Workspace& workspace = registry->GetEditorResource<Workspace>();
  std::string root = workspace.relativeProjectRoot->path.string();

  // Root is empty, dont try to compile
  // TODO : Potentially give the user a warning
  // that the game is trying to compile nothing
  if (root.length() == 0)
  {
    std::cout << "Open a workspace." << std::endl;
    return;
  }

  std::string convertedRoot = ConvertFsToNativePaths(root);

  std::cout << "relative path : " << convertedRoot << std::endl;

  std::string builder;
  builder += "cd ../generic-build && cmake -GNinja -DDEBUG=0 -DRELEASE=1 ";
  builder += "-DROOT=";
  builder += convertedRoot;
  builder += " ";
  builder += "-DCMAKE_BUILD_TYPE=Debug .. && ";
  builder += "ninja generic";

  if (fs::exists("../generic-build/generic.exe"))
  {
    std::cout << "removing generic.exe" << std::endl;
    fs::remove("generic.exe");
  }

  std::cout << builder << std::endl;
  system(builder.c_str());
}

static void CompileUserGameRelease(Registry* registry)
{
  Workspace& workspace = registry->GetEditorResource<Workspace>();
  std::string root = workspace.relativeProjectRoot->path.string();

  // Root is empty, dont try to compile
  // TODO : Potentially give the user a warning
  // that the game is trying to compile nothing
  if (root.length() == 0)
  {
    std::cout << "Open a workspace." << std::endl;
    return;
  }

  std::string convertedRoot = ConvertFsToNativePaths(root);

  std::cout << "relative path : " << convertedRoot << std::endl;

  std::string builder;
  builder += "cd ../generic-build && cmake -GNinja -DDEBUG=0 -DRELEASE=1 ";
  builder += "-DROOT=";
  builder += convertedRoot;
  builder += " ";
  builder += "-DCMAKE_BUILD_TYPE=Release .. && ";
  builder += "ninja generic";

  if (fs::exists("../generic-build/generic.exe"))
  {
    std::cout << "removing generic.exe" << std::endl;
    fs::remove("generic.exe");
  }

  std::cout << builder << std::endl;
  system(builder.c_str());
}

static void RunUserGame()
{
  system("cd ../generic-build && generic");
}

static void CopyDataBinary(std::string workspaceRoot, std::string workspacePathFile)
{
  // If it exists, then remove it as fs::copy will error out
  // copying to a file that already exists.
  // if (fs::exists("../generic-build/data.json"))
  if (fs::exists(workspacePathFile))
  {
    fs::remove(workspacePathFile);
  }

  // TODO : Binary data name standardise
  fs::copy("data", workspaceRoot);
}

static void CopyDataJson(std::string workspacePathFile, std::string workspaceRoot)
{
  // If it exists, then remove it as fs::copy will error out
  // copying to a file that already exists.
  // if (fs::exists("../generic-build/data.json"))
  if (fs::exists(workspacePathFile))
  {
    fs::remove(workspacePathFile);
  }

  fs::copy("data.json", workspaceRoot);
}

static void CopyDataJson(std::string destination)
{
  if (fs::exists(destination))
  {
    fs::remove(destination);
  }

  fs::copy("data.json", destination);
}