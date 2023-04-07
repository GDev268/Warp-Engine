#ifndef SPEACKER_GAME
#define SPEACKER_GAME

#include "speackerEngine/Common.hpp"
#include "speackerEngine/Window.hpp"

namespace Warp
{
    class Game
    {
        public:
        GLFWwindow* window;

        Game(const char* title, int width, int height, int framerate);
        ~Game() {};

        void processInput();
        void init();
        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    };
}

#endif