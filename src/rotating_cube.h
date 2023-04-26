#ifndef ROTATING_CUBE_H
#define ROTATING_CUBE_H

#include <glad/glad.h>

#include "rendering_program.h"

class RotatingCube {
 public:
  RotatingCube() {}

  ~RotatingCube() {}

  void Initialize();

  void Terminate();

  void Update();

  void Render();

 private:
  RenderingProgram program;
  GLuint vao, vbo, ebo;
};

#endif