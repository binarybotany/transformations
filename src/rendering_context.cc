#include "rendering_context.h"

void RenderingContext::Initialize() {
  WindowData window_data{"Transformations", 800, 600};

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      throw std::runtime_error("Unable to initialize context");
    }

  } catch (std::runtime_error& error) {
    std::cerr << error.what() << std::endl;
  }

  glViewport(0, 0, window_data.width, window_data.height);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback((GLDEBUGPROC)DebugMessageCallback, nullptr);
}

void RenderingContext::Terminate() {
  if (window != nullptr) {
    glfwDestroyWindow(window);
  }

  glfwTerminate();
}

void RenderingContext::Loop() {
  RotatingCube cube{};
  cube.Initialize();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cube.Update();
    cube.Render();

    glfwSwapBuffers(window);
  }

  cube.Terminate();
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

void RenderingContext::DebugMessageCallback(GLenum source, GLenum type,
                                            GLuint id, GLenum severity,
                                            GLsizei length,
                                            const GLchar* message,
                                            const void* userParam) {
  std::cout << message << std::endl;
}