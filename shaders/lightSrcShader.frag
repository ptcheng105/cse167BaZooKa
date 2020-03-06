#version 330 core

in vec3 lightColor;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;


void main()
{
    fragColor = vec4(lightColor, 1.0);
}