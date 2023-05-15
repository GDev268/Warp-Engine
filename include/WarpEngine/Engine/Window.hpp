#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include "WarpEngine/Common.hpp"

namespace WarpEngine
{
    class Window
    {
        public:
        Window(const char *title, int width, int height);
        ~Window();

        VkExtent2D getExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        bool resetWindowResizedFlag() { frameBufferResized = false;}
        GLFWwindow* getWindow() const { return window;}

        void createWindowSurface(VkInstance instance,VkSurfaceKHR *surface);

        GLFWwindow *window;

        int width, height;
        bool frameBufferResized = false;

        std::string windowname;

        private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
    };
}

#endif