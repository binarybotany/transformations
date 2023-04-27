#include "rendering_program.h"

RenderingProgram::RenderingProgram() { id = glCreateProgram(); }

RenderingProgram::~RenderingProgram() { glDeleteProgram(id); }

void RenderingProgram::AddShader(GLenum type, const GLchar *source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint compiled = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_FALSE) {
    GLsizei loglength = 0;
    GLchar message[1024];

    glGetShaderInfoLog(shader, 1024, &loglength, message);
    std::cerr << message << std::endl;

    glDeleteShader(shader);
    return;
  }

  glAttachShader(id, shader);
  shaders.push_back(shader);
}

void RenderingProgram::Link() {
  glLinkProgram(id);

  GLint linked = GL_FALSE;
  glGetProgramiv(id, GL_LINK_STATUS, &linked);
  if (linked == GL_FALSE) {
    GLsizei loglength = 0;
    GLchar message[1024];

    glGetProgramInfoLog(id, 1024, &loglength, message);
    std::cerr << message << std::endl;
  }

  for (GLuint shader : shaders) {
    glDeleteShader(shader);
  }
}

void RenderingProgram::Use() { glUseProgram(id); }

GLuint RenderingProgram::Id() { return id; }