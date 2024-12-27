#version 330 core

//in vec2 UV;

//uniform sampler2D myTextureSampler;

struct Material {
    vec4 colour;
};

in vec2 UV;

uniform Material material;

uniform sampler2D textureSampler;


out vec4 color;
void main(){
    //color = vec4(1, 0, 0, 1);
    color = texture( textureSampler, UV * 8 ).rgba;
}
