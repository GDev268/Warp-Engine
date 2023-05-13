#include <iostream>
#include <string>

#include "WarpEngine/Common.hpp"
#include "WarpEngine/Engine/Window.hpp"

int main(char argc, char **argv){
  
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    WarpEngine::Window window = WarpEngine::Window("Warp Engine",1280,720);

    while(!glfwWindowShouldClose(window.window)){
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}