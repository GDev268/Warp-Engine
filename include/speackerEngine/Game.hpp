#ifndef SPEACKER_GAME
#define SPEACKER_GAME

#include "speackerEngine/Common.hpp"
#include "speackerEngine/Window.hpp"

namespace Speacker
{
    class Game
    {
        public:
        Window* window;

        Game();
        Game(const char* title, int width, int height, int framerate);
        ~Game() {};

        void processInput();
        void init();
    };
}

#endif