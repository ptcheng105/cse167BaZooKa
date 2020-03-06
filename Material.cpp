#include "Material.h"

Material::Material(int mattype, GLuint program) {
	if (mattype == 1) {
		//just specular
		color = glm::vec3(1, 0, 0);
		specularStrength = 1;
		diffuseStrength = 0;
		shininess = 32;
		ambientStrength = 0.05;
	}
	else if (mattype == 2) {
		//just diffuse
		color = glm::vec3(0, 1, 0);
		specularStrength = 0;
		diffuseStrength = 1;
		shininess = 2;
		ambientStrength = 0.05;
	}
	else {
		color = glm::vec3(0.5, 0, 1);
		diffuseStrength = 0.5;
		specularStrength = 1;
		shininess = 32;
		ambientStrength = 0.05;
	}

	ambient = ambientStrength * color;
	diffuse = diffuseStrength * color;
	specular = specularStrength * color;
}