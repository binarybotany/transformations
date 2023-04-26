#ifndef RENDERING_CONTEXT_H
#define RENDERING_CONTEXT_H

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <exception>
#include <iostream>

#include "window_data.h"
#include "rotating_cube.h"

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

  GLFWwindow* window = nullptr;
};

#endif