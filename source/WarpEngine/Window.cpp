#include "WarpEngine/Engine/Window.hpp"

namespace WarpEngine{
    Window::Window(const char* title,int width,int height) : width(width), height(height){
        
        window = glfwCreateWindow(width,height,title,nullptr,nullptr);

    }

    Window::~Window(){
        glfwDestroyWindow(window);
    };


};