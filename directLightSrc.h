#ifndef _DIRECTLIGHTSRC_H_
#define _DIRECTLIGHTSRC_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <glm/gtx/rotate_vector.hpp>

class directLightSrc
{
private:
	glm::vec3 saved_ambient;
	glm::vec3 saved_diffuse;
	glm::vec3 saved_specular;

	bool directLightOn;
public:
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	directLightSrc(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~directLightSrc();
	void toggleSwitch();
	void update();
};

#endif