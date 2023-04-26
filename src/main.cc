#include "rendering_context.h"

int main(int argc, char** argv) {
  RenderingContext context{};

  context.Initialize();
  context.Loop();
  context.Terminate();

  return 0;
}