#ifndef RENDERING_CONTEXT_H
#define RENDERING_CONTEXT_H

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <exception>
#include <iostream>

#include "rotating_cube.h"
#include "window_data.h"

class RenderingContext {
 public:
  RenderingContext() {}

  ~RenderingContext() {}

  void Initialize();

  void Terminate();

  void Loop();

 private:
  static void FramebufferSizeCallback(GLFWwindow* window, int width,
                                      int height);

  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
                          int mods);

  static void DebugMessageCallback(GLenum source, GLenum type, GLuint id,
                                   GLenum severity, GLsizei length,
                                   const GLchar* message,
                                   const void* userParam);

  GLFWwindow* window = nullptr;
};

#endif