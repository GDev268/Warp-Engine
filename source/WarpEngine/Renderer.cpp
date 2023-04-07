#include "WarpEngine/Application/Renderer.hpp"

Warp::Renderer::Renderer(int width, int height, const char *title, bool fullscreen, bool resizable, bool wireframeMode)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE,int());
    glfwWindowHint(GLFW_MAXIMIZED,GL_TRUE);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);
}
