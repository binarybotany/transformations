#ifndef RENDERING_PROGRAM_H
#define RENDERING_PROGRAM_H

#include <glad/glad.h>

#include <iostream>
#include <vector>

class RenderingProgram {
 public:
  RenderingProgram();
  ~RenderingProgram();

  void AddShader(GLenum type, const GLchar *source);

  void Link();

  void Use();

  GLuint Id();

 private:
  GLuint id;

  std::vector<GLuint> shaders;
};

#endif