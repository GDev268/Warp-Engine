#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 texColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main(){
    //FragColor = mix(texture(texture0,texCoord),texture(texture1,texCoord)) * vec4(texColor * 1.0f);
    FragColor = vec4(texColor,1.0f);
};   

