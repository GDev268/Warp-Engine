#include "WarpEngine/Graphics/Shader.hpp"


Warp::Shader::Shader(const char* vertexShader, const char* fragmentShader)
{
    vertexShaders.push_back(loadShader(vertexShader,GL_VERTEX_SHADER));
    fragmentShaders.push_back(loadShader(fragmentShader,GL_FRAGMENT_SHADER));

    program = glCreateProgram();
    for(GLuint curShader : vertexShaders){
        glAttachShader(program,curShader);
        glDeleteShader(curShader);
    }
    for(GLuint curShader : fragmentShaders){
        glAttachShader(program,curShader);
        glDeleteShader(curShader);
    }

    glLinkProgram(program);

}

Warp::Shader::Shader()
{
    vertexShaders.push_back(loadShader("shaders/DefaultShaderVertex.glsl",GL_VERTEX_SHADER));
    fragmentShaders.push_back(loadShader("shaders/DefaultShaderFragment.glsl",GL_FRAGMENT_SHADER));

    program = glCreateProgram();
    for(GLuint curShader : vertexShaders){
        glAttachShader(program,curShader);
        glDeleteShader(curShader);
    }
    for(GLuint curShader : fragmentShaders){
        glAttachShader(program,curShader);
        glDeleteShader(curShader);
    }

    glLinkProgram(program);
}

std::string Warp::Shader::getShaderSrc(const char *filepath)
{
    std::ifstream file;
    std::stringstream buffer;

    std::string ret = "";

    file.open(filepath);

    if(file.is_open())
    {
        buffer << file.rdbuf();
        ret = buffer.str();
    }
    else{
        std::cerr << "Could not open file: " << filepath << std::endl;
    }

    return ret;
}

GLuint Warp::Shader::loadShader(const char* path, GLenum type)
{
    GLuint curShader = glCreateShader(type);
    std::string curShaderSrc = getShaderSrc(path);
    const GLchar* shader = curShaderSrc.c_str();
    glShaderSource(curShader,1,&shader,NULL);
    glCompileShader(curShader);

    checkCompilation(curShader);
    return curShader;    
}

void Warp::Shader::addShader(const char *filpath, GLenum type)
{
    if(type == GL_VERTEX_SHADER){
        vertexShaders.push_back(loadShader(filpath,GL_VERTEX_SHADER));
        updateProgram();
    }
    else if(type == GL_FRAGMENT_SHADER){
        fragmentShaders.push_back(loadShader(filpath,GL_FRAGMENT_SHADER));
        updateProgram();
    }
    else{
        std::cerr << "Invalid shader type!" << std::endl;
    }
}

void Warp::Shader::updateProgram()
{
    for(GLuint curShader : vertexShaders){
        glAttachShader(program,curShader);
        glDeleteShader(curShader);
    }
    for(GLuint curShader : fragmentShaders){
        glAttachShader(program,curShader);
        glDeleteShader(curShader);
    }

    glLinkProgram(program);
}

void Warp::Shader::checkCompilation(GLuint shader)
{
    int success;
	char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Error with vertex shader comp.:" << std::endl
				  << infoLog << std::endl;
	}
}

void Warp::Shader::setShaderValue(std::string name, int value)
{
    glUniform1i(glGetUniformLocation(program,name.c_str()),value);
}

void Warp::Shader::setShaderValue(std::string name, float value)
{
    glUniform1f(glGetUniformLocation(program,name.c_str()),value);
}

void Warp::Shader::setShaderValue(std::string name, float value1,float value2,float value3)
{
    glUniform3f(glGetUniformLocation(program,name.c_str()),value1,value2,value3);

}

void Warp::Shader::setShaderValue(std::string name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(program,name.c_str()),1,GL_FALSE,glm::value_ptr(value));
}
