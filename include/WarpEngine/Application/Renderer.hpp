#include "./Common.hpp"

namespace Warp
{
    class Renderer
    {
    public:
        Renderer(int width, int height, const char *title, bool fullscreen = false, bool resizable = false, bool wireframeMode = false);
        Renderer();
        ~Renderer();
        void addIcon(const char *path);

        unsigned char iconData;
        int width;
        int height;
        bool fullscreen;
        bool resizable;
        bool wireFrameMode;

        unsigned int VAO, VBO, IBO;

        GLFWwindow *window;
    };
}