#include "light_cube.h"

void LightCube::Initialize() {
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void LightCube::Terminate() {
  if (program != nullptr) delete program;
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void LightCube::Update(const glm::vec3 &light_position) {
  glm::mat4 model{1.0f};
  model = glm::translate(model, light_position);
  model = glm::scale(model, glm::vec3(0.2f));

  program->SetModel(model);
  program->SetView(Camera::Instance()->View());
  program->SetProjection(Camera::Instance()->Projection());
}

void LightCube::Render() {
  program->Use();
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

const GLchar *LightCube::vss = R"(
#version 330 core

layout (location = 0) in vec3 a_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0);
}
)";

const GLchar *LightCube::fss = R"(
#version 330 core

out vec4 fragment_color;

void main() {
    fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
}
)";