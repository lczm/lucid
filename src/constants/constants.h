#pragma once

// Note : all this assumes that we are in the build dir running the binary.
// All paths are relative to the build dir.
const char GLSL_VERSION[] = "#version 330";

const int MOUSE_LEFT = 0;
const int MOUSE_RIGHT = 1;
const int MOUSE_MIDDLE = 2;

const float CAMERA_SPEED = 5.0f;
const float CAMERA_SENSITIVITY = 0.05f;

// Screen values, can deal with resizing some other day
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const char TRIANGLE_VERTEX_SHADER[] = "../src/shaders/triangle.vert";
const char TRIANGLE_FRAGMENT_SHADER[] = "../src/shaders/triangle.frag";

const char MODEL_VERTEX_SHADER[] = "../src/shaders/model.vert";
const char MODEL_FRAGMENT_SHADER[] = "../src/shaders/model.frag";

// Models
const char MICROPHONE_MODEL[] = "../assets/microphone/scene.gltf";
const char SCIFIHELMET_MODEL[] = "../assets/helmet/ScifiHelmet.gltf";
const char FOX_MODEL[] = "../assets/fox/Fox.gltf";
const char AVOCADO_MODEL[] = "../assets/avocado/Avocado.gltf";