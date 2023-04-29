#include "rotating_cube.h"

#include <iostream>

void RotatingCube::Initialize() {
  srand(time(NULL));
  random_number = (float)(rand() % 10 + 1) / 10;

  program = new RenderingProgram();

  program->AddShader(GL_VERTEX_SHADER, vss);
  program->AddShader(GL_FRAGMENT_SHADER, fss);

  program->Link();
  program->Use();

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  Texture();
}

void RotatingCube::Terminate() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void RotatingCube::Update() {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::rotate(
      model, (float)glfwGetTime() * random_number * glm::radians(50.0f),
      glm::vec3(0.5f, 1.0f, 0.0f));

  GLuint u_model = glGetUniformLocation(program->Id(), "model");
  glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model));

  GLuint u_view = glGetUniformLocation(program->Id(), "view");
  glUniformMatrix4fv(u_view, 1, GL_FALSE,
                     glm::value_ptr(Camera::Instance()->View()));

  GLuint u_projection = glGetUniformLocation(program->Id(), "projection");
  glUniformMatrix4fv(u_projection, 1, GL_FALSE,
                     glm::value_ptr(Camera::Instance()->Projection()));
}

void RotatingCube::Render() {
  program->Use();
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RotatingCube::Texture() {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);

  GLint texture_width, texture_height, texture_channels;
  unsigned char *texture_data =
      stbi_load("C:\\engines\\transformations\\img\\texture.jpg",
                &texture_width, &texture_height, &texture_channels, 0);

  if (texture_data != NULL) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cerr << "Unable to load texture\n";
  }

  stbi_image_free(texture_data);
}

const GLchar *RotatingCube::vss = R"(
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texture_coords;

out vec2 texture_coords;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view; 
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(a_position, 1.0);
  texture_coords = a_texture_coords;
}
)";

const GLchar *RotatingCube::fss = R"(
#version 330 core

in vec2 texture_coords;

out vec4 fragment_color;

uniform sampler2D texture_sampler;

void main() {
  fragment_color = texture(texture_sampler, texture_coords);
}
)";