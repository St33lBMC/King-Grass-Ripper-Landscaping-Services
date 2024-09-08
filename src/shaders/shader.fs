#version 430 core

//in vec2 UV;

//uniform sampler2D myTextureSampler;

struct Material {
    vec4 colour;
};

uniform Material material;

out vec4 color;
void main(){
    color = material.colour; //texture( myTextureSampler, UV ).rgb;
}
