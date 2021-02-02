#pragma once

#include <string>
#include <stdlib.h>

#include "ecs.h"
#include "engineComponents.h"

static void CompileUserGame(Registry* registry)
{
  Workspace& workspace = registry->GetResource<Workspace>();
  std::string root = workspace.absoluteProjectRoot->path.string();

  // Root is empty, dont try to compile
  // TODO : Potentially give the user a warning
  // that the game is trying to compile nothing
  if (root.length() == 0)
  {
    return;
  }

  system(
      "cd ../generic-build && cmake -GNinja -DDEBUG=0 -DRELEASE=1 -DCMAKE_BUILD_TYPE=Release .. && "
      "ninja generic");
}

static void RunUserGame()
{
  system("cd../generic-build && generic");
}