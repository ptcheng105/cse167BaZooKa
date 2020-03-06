#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material {
public:
	glm::vec3 color;
	GLfloat ambientStrength;
	GLfloat diffuseStrength;
	GLfloat specularStrength;
	
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	GLint shininess;

	Material(int mattpye, GLuint program);
};

#endif