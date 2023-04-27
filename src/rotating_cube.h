#ifndef ROTATING_CUBE_H
#define ROTATING_CUBE_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rendering_program.h"

class RotatingCube {
 public:
  RotatingCube() {}

  ~RotatingCube() {}

  void Initialize();

  void Terminate();

  void Update();

  void Render();

  void Texture();

 private:
  RenderingProgram program;
  GLuint vao, vbo, ebo, texture;

  GLfloat vertices[32] = {
      0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f,  1.0f, 0.5f, -0.5f, 0.0f,
      0.0f, 1.0f, 0.0f,  1.0f, 0.0f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f,
      0.0f, 0.0f, -0.5f, 0.5f, 0.0f, 1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
  };

  GLuint indices[6] = {0, 1, 3, 1, 2, 3};

  static const char *vss;
  static const char *fss;
};

#endif