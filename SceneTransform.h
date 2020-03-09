#ifndef _SCENETRANSFORM_H_
#define _SCENETRANSFORM_H_
#include "SceneNode.h"
class SceneTransform :
	public SceneNode
{
private:
	std::vector<SceneNode*> childNodes;
public:
	glm::mat4 M;
	SceneTransform(glm::mat4 MT);
	virtual ~SceneTransform();

	void addChild(SceneNode* node);
	void removeChild(SceneNode* node);

	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::mat4 C);
	void update(glm::mat4 C);
	std::vector<float> getXYZMaxMin(glm::mat4 C);
};

#endif