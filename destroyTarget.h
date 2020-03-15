#ifndef _DESTROYTARGET_H_
#define _DESTROYTARGET_H_
#include "SceneObject.h"
#include "SceneGeometry.h"
#include "Window.h"
class destroyTarget : public SceneObject
{
public:
	SceneTransform* rocketHead, * rocketBody, * jet_flame,
		* jet_flame_base, * jet_flame_spike_1, * jet_flame_spike_2,
		* jet_flame_spike_3, * jet_flame_spike_4;

	glm::vec3 origin_in_world = glm::vec3(0, 0, 0);
	std::vector<float> XYZMaxMin;
	destroyTarget(glm::vec3 position_in_world, GLuint colorProg, GLuint hitbox_prog);
	~destroyTarget();
};

#endif