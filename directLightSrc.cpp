#include "directLightSrc.h"



directLightSrc::directLightSrc(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->direction = direction;
	this->saved_ambient = ambient;
	this->saved_diffuse = diffuse;
	this->saved_specular = specular;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->directLightOn = true;
}


directLightSrc::~directLightSrc()
{

}

void directLightSrc::toggleSwitch() {
	if (directLightOn) {
		ambient = glm::vec3(0);
		diffuse = glm::vec3(0);
		specular = glm::vec3(0);
		directLightOn = false;
	}
	else {
		ambient = saved_ambient;
		diffuse = saved_diffuse;
		specular = saved_specular;
		directLightOn = true;
	}
}

void directLightSrc::update() {
	direction = glm::rotateY(direction, 0.01f);
}