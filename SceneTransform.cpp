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

std::vector<float> SceneTransform::getXYZMaxMin(glm::mat4 C) {
	float xMax, yMax, zMax, xMin, yMin, zMin;
	xMax = yMax = zMax = -FLT_MAX;
	xMin = yMin = zMin = FLT_MAX;
	std::vector<float> newXYZMaxMin;
	for (int i = 0; i < childNodes.size(); i++) {
		newXYZMaxMin = childNodes[i]->getXYZMaxMin(C * M);
		if (newXYZMaxMin[0] > xMax) { xMax = newXYZMaxMin[0]; };
		if (newXYZMaxMin[1] < xMin) { xMin = newXYZMaxMin[1]; };
		if (newXYZMaxMin[2] > yMax) { yMax = newXYZMaxMin[2]; };
		if (newXYZMaxMin[3] < yMin) { yMin = newXYZMaxMin[3]; };
		if (newXYZMaxMin[4] > zMax) { zMax = newXYZMaxMin[4]; };
		if (newXYZMaxMin[5] < zMin) { zMin = newXYZMaxMin[5]; };
	}

	return { xMax ,xMin, yMax, yMin, zMax, zMin };
}