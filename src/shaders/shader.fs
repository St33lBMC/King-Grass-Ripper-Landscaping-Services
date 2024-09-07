#version 430 core

//in vec2 UV;

//uniform sampler2D myTextureSampler;
uniform vec3 incolo
out vec4 color;
void main(){
    color = vec4(0.5, 0.5, 0.5, 1);//texture( myTextureSampler, UV ).rgb;
}
