#version 430 core

//in vec2 UV;

//uniform sampler2D myTextureSampler;
out vec4 color;
void main(){
    color = vec4(0.0, 0.0, 1, 1);//texture( myTextureSampler, UV ).rgb;
}