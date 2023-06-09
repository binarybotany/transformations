#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mutex>

class Camera {
 public:
  Camera(Camera &) = delete;
  void operator=(const Camera &) = delete;

  static Camera *Instance();

  void Initialize(int width, int height);

  void Terminate();

  void MoveLeft(float speed);

  void MoveRight(float speed);

  void MoveForwards(float speed);

  void MoveBackwards(float speed);

  void SetDirection(glm::vec3 value);

  void SetFieldOfView(float value);

  glm::mat4 View() const;

  glm::mat4 Projection() const;

  glm::vec3 Position() const;

 protected:
  Camera() {}
  ~Camera() {}

 private:
  static Camera *instance;
  static std::mutex mutex;

  float width = 1.0f, height = 1.0f;

  glm::vec3 position{1.0f};
  glm::vec3 front{1.0f};
  glm::vec3 up{1.0f};

  float fov = 45.0f;
};

#endif