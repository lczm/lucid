#pragma once

#include <stdlib.h>

static void CompileUserGame()
{
  system("cmake -GNinja -DDEBUG=0 -DRELEASE=1 -DCMAKE_BUILD_TYPE=Release .. && ninja generic");
}

static void RunUserGame()
{
  system("generic");
}