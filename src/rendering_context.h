#ifndef RENDERING_CONTEXT_H
#define RENDERING_CONTEXT_H

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <exception>
#include <iostream>
#include <vector>

#include "camera.h"
#include "cube.h"
#include "light_cube.h"
#include "material.h"
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

  static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

  static void ScrollCallback(GLFWwindow* window, double xoffset,
                             double yoffset);

  static void DebugMessageCallback(GLenum source, GLenum type, GLuint id,
                                   GLenum severity, GLsizei length,
                                   const GLchar* message,
                                   const void* userParam);

  void ProcessInput();

  void CalculateDeltaTime();

  GLFWwindow* window = nullptr;

  float delta_time = 0.0f;
  float last_frame = 0.0f;

  static float yaw;
  static float pitch;

  static float last_x;
  static float last_y;

  int width, height;
};

#endif