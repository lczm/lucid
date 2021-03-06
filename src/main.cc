#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#define GLAD_DEBUG

#include <iostream>
#include <glad/gl.h>

#include "lucid.h"
#include "constants.h"
#include "ecs.h"
#include "input.h"
#include <GLFW/glfw3.h>

static void ErrorCallback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

static bool GlCheckError()
{
  while (GLenum error = glGetError())
  {
    std::cout << "GL Error : " << std::hex << error << std::endl;
    return true;
  }
  return false;
}

void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                     const GLchar* message, const void* userParam)
{
  // This is the wide glLineWidth depreciation warning.
  if (type == 0x824d)
  {
    return;
  }

  // 0x826B is just a notification, which is what is printing out most of the
  // time. To keep the output verbose, only print out when there is a explicitly
  // defined severity error.
  if (severity == 0x9146 ||  // Severity_High
      severity == 0x9147 ||  // Severity_Medium
      severity == 0x9148)
  {  // Severity_Low
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
  }
}

// Explicitly turn on vsync through GLFW
void EnableVSync()
{
  glfwSwapInterval(1);
}

// To disable vsync, on computers that do not have the GPU enforcing
// vsync on game windows (laptops), this will make it so that it
// possibly goes on (hundreds) of frames per second.
void DisableVSync()
{
  glfwSwapInterval(0);
}

int main(void)
{
  GLFWwindow* window;

  glfwSetErrorCallback(ErrorCallback);

  if (!glfwInit())
  {
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lucid", NULL, NULL);
  // Window Icon
  GLFWimage images[1];
  images[0].pixels =
      stbi_load(AIRMAN_IMAGE, &images[0].width, &images[0].height, 0, 4);  // rgba channels
  glfwSetWindowIcon(window, 1, images);
  stbi_image_free(images[0].pixels);

  if (!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // A context can only be made current on a single thread at any one time.
  // You can move contexts around windows, but for now we can just leave
  // this here, this sets the current window as the context current
  glfwMakeContextCurrent(window);

  // EnableVSync();
  DisableVSync();

  // Load gl with glad
  gladLoadGL(glfwGetProcAddress);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  // Note : for whatever reason, setting this will crash on windows + gpu, windows + igpu will work
  // perfectly fine
  // glDebugMessageCallback(MessageCallback, nullptr);
  // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL,
  //                       GL_FALSE);

  std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  // Tell OpenGL to depth test
  glEnable(GL_DEPTH_TEST);

  // stbi_set_flip_vertically_on_load(true);

  Registry* registry = new Registry();
  Input* input = new Input(window);
  Lucid* lucid = new Lucid(registry, input, window);

  while (!glfwWindowShouldClose(window))
  {
    // Flip buffers
    glfwSwapBuffers(window);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Handle events -> inputs, resize, etc.
    glfwPollEvents();

    // Clear all errors, so that when we check for errors, the
    // errors are not checking the previous iteration
    while (glGetError() != GL_NO_ERROR)
      ;

    lucid->Update();

    if (GlCheckError())
    {
      // break out of the loop
      std::cout << "Breaking out of the loop, OpenGL Error" << std::endl;
      break;
    }
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
