#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 vertexUV;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;

out vec2 UV;

void main(void)
{
		vec4 pos1 = vec4(position, 1);
		gl_Position = (projection_matrix * view_matrix * model_matrix * pos1);
		UV = vertexUV;

}
