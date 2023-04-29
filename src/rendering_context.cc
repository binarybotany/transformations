#include "rendering_context.h"

float RenderingContext::yaw = -90.0f;
float RenderingContext::pitch = 0.0f;

float RenderingContext::last_x = 0.0f;
float RenderingContext::last_y = 0.0f;

void RenderingContext::Initialize() {
  WindowData window_data{"Transformations", 1024, 768};

  last_x = ((float)window_data.width) / 2;
  last_y = ((float)window_data.height) / 2;

  try {
    if (!glfwInit()) {
      throw std::runtime_error("Unable to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_data.width, window_data.height,
                              window_data.title.c_str(), nullptr, nullptr);
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

  glViewport(0, 0, window_data.width, window_data.height);

  glEnable(GL_DEPTH_TEST);

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
  Camera::Instance()->Initialize();

  std::vector<RotatingCube> cubes;

  glm::vec3 cube_positions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  for (auto position : cube_positions) {
    RotatingCube cube{position};
    cube.Initialize();
    cubes.push_back(cube);
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    CalculateDeltaTime();

    ProcessInput();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto cube : cubes) {
      cube.Update();
      cube.Render();
    }

    glfwSwapBuffers(window);
  }

  for (auto cube : cubes) {
    cube.Terminate();
  }

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