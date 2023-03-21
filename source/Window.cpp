#include "Window.hpp"
#include <GLFW/glfw3.h>


Window::Window(){
    window = glfwCreateWindow(1280,720,"Speacker Engine",NULL,NULL);
}

Window::~Window(){

}
