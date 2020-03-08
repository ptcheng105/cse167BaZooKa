#ifndef POLETARGET_H_
#define POLETARGET_H_
#include "SceneObject.h"
#include "Window.h"
class PoleTarget :
	public SceneObject
{
private:
	SceneTransform* pole;
public:
	PoleTarget(SceneGeometry* cylinder);
	PoleTarget(glm::mat4 MT, SceneGeometry* cylinder);
	~PoleTarget();
	void initPole(SceneGeometry* cylinder);

	void drawObject(GLuint shaderProgram, glm::mat4 C);
};

#endif