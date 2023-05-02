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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Cube::Terminate() {
  if (program != nullptr) delete program;
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void Cube::Update(const Material &material, const glm::vec3 translation,
                  const glm::vec3 &light_position) {
  glm::mat4 model{1.0f};
  model = glm::translate(model, translation);

  program->SetModel(model);
  program->SetView(Camera::Instance()->View());
  program->SetProjection(Camera::Instance()->Projection());

  program->SetVec3("light_position", light_position);
  program->SetVec3("view_position", Camera::Instance()->Position());

  program->SetVec3("material.ambient", material.ambient);
  program->SetVec3("material.diffuse", material.diffuse);
  program->SetVec3("material.specular", material.specular);
  program->SetFloat("material.shininess", material.shininess);

  program->SetVec3("light.ambient", {0.2f, 0.2f, 0.2f});
  program->SetVec3("light.diffuse", {0.5f, 0.5f, 0.5f});
  program->SetVec3("light.ambient", {1.0f, 1.0f, 1.0f});
}

void Cube::Render() {
  program->Use();
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

const GLchar *Cube::vss = R"(
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

out vec3 normal;
out vec3 fragment_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    fragment_position = vec3(model * vec4(a_position, 1.0));
    normal = a_normal;
}
)";

const GLchar *Cube::fss = R"(
#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 normal;
in vec3 fragment_position;

uniform vec3 light_position;
uniform vec3 view_position;

out vec4 fragment_color;

uniform Light light;
uniform Material material;

void main() {
    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 normalized_normal = normalize(normal);
    vec3 light_direction = normalize(light_position - fragment_position);
    float diff = max(dot(normalized_normal, light_direction), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflection_direction = reflect(-light_direction, normalized_normal);
    float spec = pow(
      max(dot(view_direction, reflection_direction), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    fragment_color = vec4(result, 1.0);
}
)";