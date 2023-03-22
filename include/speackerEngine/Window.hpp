#ifndef SPEACKER_WINDOW
#define SPEACKER_WINDOW

#include "speackerEngine/Common.hpp"

namespace Speacker
{
    class Window
    {
        public:
        Window(const char* title,int width,int height);
        ~Window() {};

        void update();
        void endFrame();

        GLFWwindow *_window;
    };

}
#endif