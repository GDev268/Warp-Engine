#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "WarpEngine/Common.hpp"

namespace WarpEngine
{
    class Window
    {
        public:
        Window(const char *title, int width, int height);
        ~Window();

        VkExtent2D getExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

        GLFWwindow *window;

        const int width, height;
    };
}

#endif