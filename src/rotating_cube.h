#ifndef ROTATING_CUBE_H
#define ROTATING_CUBE_H

#include <glad/glad.h>

class RotatingCube {
 public:
  RotatingCube() {}

  ~RotatingCube() {}

  void Initialize();

  void Terminate();

  void Update();

  void Render();

 private:
  GLuint vao, vbo, ebo;
};

#endif