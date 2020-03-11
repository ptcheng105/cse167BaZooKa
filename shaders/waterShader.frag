#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{   
    vec3 I = normalize(Position - cameraPos);

    //reflective
    vec3 R1 = reflect(I, normalize(Normal));
    vec4 reflection = 0.5 * vec4(texture(skybox, R1).rgb, 1);

    //refractive
    float ratio = 1.00 / 1.33;
    vec3 R2 = refract(I, normalize(Normal), ratio);
    vec4 refraction = 0.5 * vec4(texture(skybox, R2).rgb, 1);

    FragColor = reflection + refraction;
    //FragColor = vec4(1,0,0,1);
}