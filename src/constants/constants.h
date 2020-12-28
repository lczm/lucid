#pragma once

#define DEBUG true
#define RELEASE false

// Note : all this assumes that we are in the build dir running the binary.
// All paths are relative to the build dir.
const char GLSL_VERSION[] = "#version 330";

const int MOUSE_LEFT = 0;
const int MOUSE_RIGHT = 1;
const int MOUSE_MIDDLE = 2;

const float CAMERA_SPEED = 15.0f;
const float SCROLL_SPEED = 50.0f;
const float CAMERA_SENSITIVITY = 0.15f;

// Screen values, can deal with resizing some other day
const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

const char TRIANGLE_VERTEX_SHADER[] = "../src/shaders/triangle.vert";
const char TRIANGLE_FRAGMENT_SHADER[] = "../src/shaders/triangle.frag";

const char MODEL_VERTEX_SHADER[] = "../src/shaders/model.vert";
const char MODEL_ANIMATED_VERTEX_SHADER[] = "../src/shaders/modelAnimated.vert";
const char MODEL_FRAGMENT_SHADER[] = "../src/shaders/model.frag";

const char PRIMITIVE_VERTEX_SHADER[] = "../src/shaders/primitive.vert";
const char PRIMITIVE_LINE_SHADER[] = "../src/shaders/line.vert";
const char PRIMITIVE_INSTANCED_SHADER[] = "../src/shaders/primitiveInstanced.vert";
const char PRIMITIVE_FRAGMENT_SHADER[] = "../src/shaders/primitive.frag";

// Models
const char MICROPHONE_MODEL[] = "../assets/microphone/scene.gltf";
const char SCIFIHELMET_MODEL[] = "../assets/helmet/ScifiHelmet.gltf";
const char FOX_MODEL[] = "../assets/fox/Fox.gltf";
const char AVOCADO_MODEL[] = "../assets/avocado/Avocado.gltf";
const char POLYBIRD_MODEL[] = "../assets/low-poly-bird-animated/source/Bird_Asset.fbx";
const char POLYFOX_MODEL[] = "../assets/low-poly-fox-by-pixelmannen-animated/source/animations.fbx";
const char SHIBA_INU_MODEL[] = "../assets/animated_dog_shiba_inu_gltf/scene.gltf";
const char EAGLE_MODEL[] = "../assets/animated_eagle/scene.gltf";

// Images
const char AIRMAN_IMAGE[] = "../assets/images/air-man.png";

// Audio
const char GRUNT_SOUND[] = "../assets/audio/grunt.wav";
const char PIANO_MUSIC[] = "../assets/audio/piano.wav";

// Pseudo standard values that will not change
const float PI = static_cast<float>(3.14159265359);

// Pong demo
const float PADDLE_MOVE_SPEED = 0.05f;
