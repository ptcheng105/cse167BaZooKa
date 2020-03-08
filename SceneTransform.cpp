#include "SceneTransform.h"

SceneTransform::SceneTransform(glm::mat4 MT)
{
	M = MT;
}

SceneTransform::~SceneTransform()
{
}

void SceneTransform::addChild(SceneNode* node) {
	childNodes.push_back(node);
}

void SceneTransform::removeChild(SceneNode* node) {
	for (int i = 0; i < childNodes.size(); i++) {
		if (childNodes[i] == node) {
			childNodes.erase(childNodes.begin() + i);
			break;
		}
	}
}

void SceneTransform::draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::mat4 C) {
	for (int i = 0; i < childNodes.size(); i++) {
		childNodes[i]->draw(shaderProgram,projection,view, C * M);
	}
}

void SceneTransform::update(glm::mat4 C) {
	M = C * M;
}