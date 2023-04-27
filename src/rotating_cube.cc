#include "rotating_cube.h"

#include <iostream>

void RotatingCube::Initialize() {
  program.AddShader(GL_VERTEX_SHADER, vss);
  program.AddShader(GL_FRAGMENT_SHADER, fss);

  program.Link();
  program.Use();

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                        (GLvoid *)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  Texture();
}

void RotatingCube::Terminate() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
}

void RotatingCube::Update() {
  glm::mat4 transform = glm::mat4(1.0f);

  GLfloat scale = (GLfloat)sin(glfwGetTime());
  transform = glm::scale(transform, glm::vec3(scale, scale, scale));
  transform = glm::rotate(transform, (GLfloat)glfwGetTime(),
                          glm::vec3(0.0f, 0.0f, 1.0f));

  GLuint u_transform = glGetUniformLocation(program.Id(), "transform");
  glUniformMatrix4fv(u_transform, 1, GL_FALSE, glm::value_ptr(transform));
}

void RotatingCube::Render() {
  program.Use();
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void RotatingCube::Texture() {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

const char *RotatingCube::vss = R"(
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texture_coords;

out vec3 color;
out vec2 texture_coords;

uniform mat4 transform;

void main() {
  gl_Position = transform * vec4(a_position, 1.0);
  color = a_color;
  texture_coords = a_texture_coords;
}
)";

const char *RotatingCube::fss = R"(
#version 330 core

in vec3 color;
in vec2 texture_coords;

out vec4 fragment_color;

uniform sampler2D texture_sampler;

void main() {
  fragment_color = texture(texture_sampler, texture_coords);
}
)";