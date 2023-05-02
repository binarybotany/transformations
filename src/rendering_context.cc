#include "rendering_context.h"

float RenderingContext::yaw = -90.0f;
float RenderingContext::pitch = 0.0f;

float RenderingContext::last_x = 0.0f;
float RenderingContext::last_y = 0.0f;

void RenderingContext::Initialize() {
  try {
    if (!glfwInit()) {
      throw std::runtime_error("Unable to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);
    width = video_mode->width;
    height = video_mode->height;

    last_x = ((float)width) / 2;
    last_y = ((float)height) / 2;

    window =
        glfwCreateWindow(width, height, "Transformations", monitor, nullptr);
    if (window == nullptr) {
      throw std::runtime_error("Unable to create window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      throw std::runtime_error("Unable to initialize context");
    }

  } catch (std::runtime_error& error) {
    std::cerr << error.what() << std::endl;
  }

  glViewport(0, 0, width, height);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(DebugMessageCallback, nullptr);
}

void RenderingContext::Terminate() {
  if (window != nullptr) {
    glfwDestroyWindow(window);
  }

  glfwTerminate();
}

void RenderingContext::Loop() {
  Material emerald{{0.0215f, 0.1745f, 0.0215f},
                   {0.07568f, 0.61424f, 0.07568f},
                   {0.633f, 0.727811f, 0.633f},
                   0.6f};

  Material pearl{{0.25f, 0.20725f, 0.20725f},
                 {1.0f, 0.829f, 0.829f},
                 {0.296648f, 0.296648f, 0.296648f},
                 0.088f};

  Camera::Instance()->Initialize(width, height);

  Cube emerald_cube{};
  emerald_cube.Initialize();

  Cube pearl_cube{};
  pearl_cube.Initialize();

  LightCube light_cube{};
  light_cube.Initialize();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    CalculateDeltaTime();

    ProcessInput();

    float x = sin(glfwGetTime()) * 3.0f;
    float z = cos(glfwGetTime()) * 3.0f;
    glm::vec3 light_position{x, 1.0f, z};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    emerald_cube.Update(emerald, {-1.0f, 0.0f, 0.0f}, light_position);
    emerald_cube.Render();

    pearl_cube.Update(pearl, {1.0f, 0.0f, 0.0f}, light_position);
    pearl_cube.Render();

    light_cube.Update(light_position);
    light_cube.Render();

    glfwSwapBuffers(window);
  }

  emerald_cube.Terminate();

  pearl_cube.Terminate();

  light_cube.Terminate();

  Camera::Instance()->Terminate();
}

void RenderingContext::FramebufferSizeCallback(GLFWwindow* window, int width,
                                               int height) {
  glViewport(0, 0, width, height);
}

void RenderingContext::KeyCallback(GLFWwindow* window, int key, int scancode,
                                   int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void RenderingContext::CursorPosCallback(GLFWwindow* window, double xpos,
                                         double ypos) {
  static bool first_cursor_pos;
  if (first_cursor_pos) {
    last_x = (float)xpos;
    last_y = (float)ypos;
    first_cursor_pos = false;
  }

  float x_offset = ((float)xpos) - last_x;
  float y_offset = last_y - ((float)ypos);
  last_x = (float)xpos;
  last_y = (float)ypos;

  const float sensitivity = 0.1f;
  x_offset *= sensitivity;
  y_offset *= sensitivity;

  yaw += x_offset;
  pitch += y_offset;

  if (pitch > 89.0f) pitch = 89.0f;
  if (pitch < -89.0f) pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  Camera::Instance()->SetDirection(glm::normalize(direction));
}

void RenderingContext::ScrollCallback(GLFWwindow* window, double xoffset,
                                      double yoffset) {
  Camera::Instance()->SetFieldOfView((float)yoffset);
}

void RenderingContext::DebugMessageCallback(GLenum source, GLenum type,
                                            GLuint id, GLenum severity,
                                            GLsizei length,
                                            const GLchar* message,
                                            const void* userParam) {
  std::cout << message << std::endl;
}

void RenderingContext::ProcessInput() {
  float camera_speed = 4.0f * delta_time;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    Camera::Instance()->MoveForwards(camera_speed);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    Camera::Instance()->MoveBackwards(camera_speed);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    Camera::Instance()->MoveLeft(camera_speed);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    Camera::Instance()->MoveRight(camera_speed);
  }
}

void RenderingContext::CalculateDeltaTime() {
  float current_frame = glfwGetTime();
  delta_time = current_frame - last_frame;
  last_frame = current_frame;
}