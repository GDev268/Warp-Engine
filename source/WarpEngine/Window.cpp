#include "WarpEngine/Engine/Window.hpp"

namespace WarpEngine
{
    Window::Window(const char *title, int width, int height) : width(width), height(height)
    {
        glfwInit();
        glfwVulkanSupported();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    Window::~Window()
    {
        glfwDestroyWindow(window);
    };

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("ERROR 4: Couldn't create window surface");
        }
    }

    void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto curWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        curWindow->frameBufferResized = true;
        curWindow->width = width;
        curWindow->height = height;
    }
};