#pragma once

// Note : all this assumes that we are in the build dir running the binary.
// All paths are relative to the build dir.
const char GLSL_VERSION[] = "#version 330";

// Screen values, can deal with resizing some other day
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const char TRIANGLE_VERTEX_SHADER[] = "../src/shaders/triangle.vert";
const char TRIANGLE_FRAGMENT_SHADER[] = "../src/shaders/triangle.frag";