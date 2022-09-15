#include <iostream>
//#include <adder.h>
#include <GLFW/glfw3.h>
#include<cmake_tutorialConfig.h>


int main(int argc, char* argv[])
{
  std::cout << "Hello World!" << std::endl;
#ifdef USE_ADDER
  std::cout << "use Adder: " << add(3.14f, 4.93f) << std::endl;
#else
  std::cout << "use std: " << 3.14f + 4.93f << std::endl;
#endif
  std::cout << argv[0] << " Version: " << cmake_tutorial_VERSION_MAJOR << "."
      << cmake_tutorial_VERSION_MINOR << std::endl;

  GLFWwindow *window;
  if( !glfwInit() )
  {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      exit( EXIT_FAILURE );
  }
  window = glfwCreateWindow( 300, 300, "Gears", NULL, NULL );
  if (!window)
  {
      fprintf( stderr, "Failed to open GLFW window\n" );
      glfwTerminate();
      exit( EXIT_FAILURE );
  }
     // Main loop
  while( !glfwWindowShouldClose(window) )
  {

      // Swap buffers
      glfwSwapBuffers(window);
      glfwPollEvents();
  }

  // Terminate GLFW
  glfwTerminate();
  return 1;
 }
