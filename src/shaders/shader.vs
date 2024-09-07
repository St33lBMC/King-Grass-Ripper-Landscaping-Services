#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 model_matrix;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;


void main(void)
{
		vec4 pos1 = vec4(position, 1);
		gl_Position = (projection_matrix * view_matrix * model_matrix * pos1);

}
