#ifndef WARP_TEXTURE
#define WARP_TEXTURE

#include "./Common.hpp"

namespace Warp
{
    class Texture
    {
        public:
        Texture();
        Texture(const char* path,const char* name,bool defaultParams = true);

        void createTexture();
        void loadGraphic();
    };
}

#endif