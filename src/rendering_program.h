#ifndef RENDERING_PROGRAM_H
#define RENDERING_PROGRAM_H

#include <glad/glad.h>

class RenderingProgram {
 public:
  RenderingProgram() {}
  ~RenderingProgram() {}

  void Use();

 private:
  GLuint id;
};

#endif