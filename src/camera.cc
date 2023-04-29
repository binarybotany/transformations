#include "camera.h"

Camera* Camera::instance = nullptr;
std::mutex Camera::mutex;

Camera* Camera::Instance() {
  std::lock_guard<std::mutex> lock(mutex);

  if (instance == nullptr) {
    instance = new Camera();
  }

  return instance;
}

void Camera::Initialize() {
  position = glm::vec3(0.0f, 0.0f, 3.0f);
  front = glm::vec3(0.0f, 0.0f, -1.0f);
  up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::Terminate() {}

void Camera::MoveLeft(float speed) {
  position -= glm::normalize(glm::cross(front, up)) * speed;
}

void Camera::MoveRight(float speed) {
  position += glm::normalize(glm::cross(front, up)) * speed;
}

void Camera::MoveForwards(float speed) { position += speed * front; }

void Camera::MoveBackwards(float speed) { position -= speed * front; }

void Camera::SetDirection(glm::vec3 value) { front = value; }

void Camera::SetFieldOfView(float value) {
  fov -= value;
  if (fov < 1.0f) fov = 1.0f;
  if (fov > 45.0f) fov = 45.0f;
}

glm::mat4 Camera::View() { return glm::lookAt(position, position + front, up); }

glm::mat4 Camera::Projection() {
  return glm::perspective(glm::radians(fov), 1024.0f / 768.0f, 0.1f, 100.0f);
}