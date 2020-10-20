#include "sandbox.h"

Sandbox::Sandbox(Registry* registry, Input* input, GLFWwindow* window) {
  Sandbox::registry = registry;
  Sandbox::input = input;
  Sandbox::window = window;
}

Sandbox::~Sandbox() = default;

void Sandbox::Update(double dt) {
}