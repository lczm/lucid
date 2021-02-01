#pragma once

#include <stdlib.h>

static void CompileUserGame()
{
  system(
      "cd ../generic-build && cmake -GNinja -DDEBUG=0 -DRELEASE=1 -DCMAKE_BUILD_TYPE=Release .. && "
      "ninja generic");
}

static void RunUserGame()
{
  system("cd../generic-build && generic");
}