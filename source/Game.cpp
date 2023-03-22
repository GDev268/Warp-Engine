#include "speackerEngine/Game.hpp"

Speacker::Game::Game(const char* title, int width, int height, int framerate){
    glfwInit();
    glewInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = new Window(title,width,height);
    while(!glfwWindowShouldClose(window->_window)){
        std::cout << "updating  window" << std::endl;
    }
}

Speacker::Game::Game(){

}


void Speacker::Game::init()
{
    while(!glfwWindowShouldClose(window->_window)){
        std::cout << "updating  window" << std::endl;
    }
}   