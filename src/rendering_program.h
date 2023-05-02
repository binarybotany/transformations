#ifndef RENDERING_PROGRAM_H
#define RENDERING_PROGRAM_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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

  void SetModel(glm::mat4 model);

  void SetView(glm::mat4 view);

  void SetProjection(glm::mat4 projection);

  void SetFloat(const char *name, const GLfloat value);

  void SetMat4(const char *name, glm::mat4 value);

  void SetVec3(const char *name, glm::vec3 value);

  void CheckIfProgramInUse();

 private:
  GLuint id, u_model, u_view, u_projection;

  std::vector<GLuint> shaders;
};

#endif