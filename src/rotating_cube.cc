#include "rotating_cube.h"

#include <iostream>

void RotatingCube::Initialize() {
  program.AddShader(GL_VERTEX_SHADER, vss);
  program.AddShader(GL_FRAGMENT_SHADER, fss);

  program.Link();
  program.Use();

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void RotatingCube::Terminate() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void RotatingCube::Update() {}

void RotatingCube::Render() {
  program.Use();
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

const char *RotatingCube::vss = R"(
#version 330 core

layout (location = 0) vec3 a_positions;

void main() {
  gl_Position = vec4(a_positions.x, a_positions.y, a_positions.z, 1.0);
}
)";

const char *RotatingCube::fss = R"(
#version 330 core

out vec4 fragment_color;

void main() {
  fragment_color = vec4(0.0, 0.5, 0.5, 1.0);
}
)";