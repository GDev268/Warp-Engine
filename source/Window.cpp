#include "speackerEngine/Window.hpp"

Speacker::Window::Window(const char* title,int width,int height)
{
    _window = glfwCreateWindow(width,height,title,NULL,NULL);
    glfwMakeContextCurrent(_window);

    glViewport(0,0,1280,720);
}

void Speacker::Window::update(){
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Speacker::Window::endFrame(){
	glfwSwapBuffers(_window);
	glfwPollEvents();
}
