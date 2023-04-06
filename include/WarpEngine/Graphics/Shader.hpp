#ifndef WARP_SHADER
#define WARP_SHADER

#include "./Common.hpp"

namespace Warp{
    class Shader{
        public:
            Shader(const char* vertexShader,const char* fragmentShader);
            Shader();

            std::string getShaderSrc(const char *filepath);
            GLuint loadShader(const char* path, GLenum type);

            void addShader(const char *filpath, GLenum type);

            void updateProgram();

            void checkCompilation(GLuint shader);

            void setShaderValue(std::string name, int value);
            void setShaderValue(std::string name, float value);
            void setShaderValue(std::string name, float value1,float value2,float value3);
            void setShaderValue(std::string name, glm::mat4 value);

            std::vector<GLuint> vertexShaders;
            std::vector<GLuint> fragmentShaders;

            unsigned int program;
    };
};

#endif