#ifndef _DESTROYTARGET_H_
#define _DESTROYTARGET_H_
#include "SceneObject.h"
#include "SceneGeometry.h"
#include "Window.h"
class destroyTarget : public SceneObject
{
public:
	SceneTransform* canon_pipe, * pipe1, * pipe2, * pipe3, *pipes, *canon_base;

	glm::vec3 origin_in_world = glm::vec3(0, 0, 0);
	std::vector<float> XYZMaxMin;
	destroyTarget(glm::vec3 position_in_world, GLuint colorProg, GLuint hitbox_prog);
	~destroyTarget();
};

#endif