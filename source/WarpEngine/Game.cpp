#include "WarpEngine/Application/Game.hpp"
#include "WarpEngine/Graphics/Shader.hpp"

Warp::Game::Game(const char *title, int width, int height, int framerate)
{
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED,GL_TRUE);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window = new Window(title,width,height);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);

    GLenum verifyGlew = glewInit();
    if (verifyGlew != GLEW_OK)
    {

        std::cerr << "FAILED TO INITIALIZE GLEW" << std::endl << glewGetErrorString(verifyGlew) <<std::endl;
    }

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glViewport(0, 0, 1280, 720);
    init();
}


void Warp::Game::init()
{
    Shader* shader = new Shader();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.43f, 0.69f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Warp::Game::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{	
    glViewport(0, 0, width, height);
}