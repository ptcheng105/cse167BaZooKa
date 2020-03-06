#ifndef _SCENETRANSFORM_H_
#define _SCENETRANSFORM_H_
#include "SceneNode.h"
class SceneTransform :
	public SceneNode
{
private:
	glm::mat4 M;
	std::vector<SceneNode*> childNodes;
public:
	SceneTransform(glm::mat4 MT);
	virtual ~SceneTransform();

	void addChild(SceneNode* node);
	void removeChild(SceneNode* node);

	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 C);
};

#endif