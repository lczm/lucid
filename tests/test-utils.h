#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

static GLFWwindow* SetUpWindow() {
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lucid", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  gladLoadGL(glfwGetProcAddress);

  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  return window;
}

static void TearDownWindow(GLFWwindow* window) {
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}