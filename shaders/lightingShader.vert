#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vn;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out vec3 FragPos;  
out vec3 Normal;

void main()
{
	// OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * view * model * vec4(position, 1.0);

	FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * vn; 
}