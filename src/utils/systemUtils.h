#pragma once

#include <string>
#include <stdlib.h>

#include "ecs.h"
#include "engineComponents.h"

static void CompileUserGame(Registry* registry)
{
  Workspace& workspace = registry->GetResource<Workspace>();
  std::string root = workspace.relativeProjectRoot->path.string();

  // Root is empty, dont try to compile
  // TODO : Potentially give the user a warning
  // that the game is trying to compile nothing
  if (root.length() == 0)
  {
    std::cout << "Open a workspace." << std::endl;
    return;
  }

  std::string convertedRoot = "";
  for (auto c : convertedRoot)
  {
    if (c == '\\')
    {
      convertedRoot += "/";
    }
    else
    {
      convertedRoot += c;
    }
  }

  std::string builder;
  builder += "cd ../generic-build && cmake -GNinja -DDEBUG=0 -DRELEASE=1 ";
  builder += "-DROOT=";
  builder += convertedRoot;
  builder += " ";
  builder += "-DCMAKE_BUILD_TYPE=Release .. && ";
  builder += "ninja generic";

  std::cout << builder << std::endl;
  system(builder.c_str());
}

static void RunUserGame()
{
  system("cd../generic-build && generic");
}