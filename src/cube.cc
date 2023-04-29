#include "cube.h"

void Cube::Initialize() {
  program = new RenderingProgram();

  program->AddShader(GL_VERTEX_SHADER, vss);
  program->AddShader(GL_FRAGMENT_SHADER, fss);

  program->Link();
  program->Use();

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Cube::Terminate() {
  if (program != nullptr) delete program;
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void Cube::Update() {}

void Cube::Render() {
  program->Use();
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

const GLchar *Cube::vss = R"(
#version 330 core

layout (location = 0) in vec3 a_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0);
}
)";

const GLchar *Cube::fss = R"(
#version 330 core

out vec4 fragment_color;

uniform vec3 object_color;
uniform vec3 light_color;

void main() {
    fragment_color = vec4(light_color * object_color, 1.0);
}
)";