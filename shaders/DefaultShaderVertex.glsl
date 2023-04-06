#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 fragColor;
layout(location = 3) in vec2 fragCoord;

out vec2 texCoord;
out vec3 texColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
    gl_Position = model * vec4(vertexPos,1.0);
    texColor = fragColor;
    texCoord = fragCoord;
};