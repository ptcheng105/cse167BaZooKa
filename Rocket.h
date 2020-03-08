#ifndef _ROCKET_H_
#define _ROCKET_H_
#include "SceneObject.h"
#include "Window.h"
class Rocket :
	public SceneObject
{
private:
	SceneTransform* rocketHead, * rocketBody, * jet_flame,
		* jet_flame_base, * jet_flame_spike_1, * jet_flame_spike_2, 
		* jet_flame_spike_3, * jet_flame_spike_4;
	glm::vec3 velocity = glm::vec3(0, 0, -0.001);
public:
	Rocket(SceneGeometry* cylinder, SceneGeometry* cone, SceneGeometry* sphere);
	Rocket(glm::mat4 MT, SceneGeometry* cylinder, SceneGeometry* cone, SceneGeometry* sphere);
	~Rocket();
	void initRocket(SceneGeometry* cylinder, SceneGeometry* cone, SceneGeometry* sphere);

	void drawObject(GLuint shaderProgram, glm::mat4 C);
	void move();
};

#endif