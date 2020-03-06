#ifndef _SCENEOBJECT_H_
#define _SCENEOBJECT_H_
#include "SceneNode.h"
#include "SceneTransform.h"
#include "SceneGeometry.h"

class SceneObject :
	public SceneNode
{
private:
	SceneTransform * object_model_transform;
public:
	SceneObject(glm::mat4(MT));
	virtual ~SceneObject();
};

#endif