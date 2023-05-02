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

  u_model = glGetUniformLocation(id, "model");
  u_view = glGetUniformLocation(id, "view");
  u_projection = glGetUniformLocation(id, "projection");
}

void RenderingProgram::Use() { glUseProgram(id); }

GLuint RenderingProgram::Id() { return id; }

void RenderingProgram::SetModel(glm::mat4 model) {
  Use();
  glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model));
}

void RenderingProgram::SetView(glm::mat4 view) {
  Use();
  glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));
}

void RenderingProgram::SetProjection(glm::mat4 projection) {
  Use();
  glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection));
}

void RenderingProgram::SetFloat(const char *name, const GLfloat value) {
  Use();
  glUniform1f(glGetUniformLocation(id, name), value);
}

void RenderingProgram::SetMat4(const char *name, glm::mat4 value) {
  Use();
  glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void RenderingProgram::SetVec3(const char *name, glm::vec3 value) {
  Use();
  glUniform3fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
}