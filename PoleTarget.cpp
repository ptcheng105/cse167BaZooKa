#include "PoleTarget.h"
PoleTarget::PoleTarget(SceneGeometry* cylinder) {
	initPole(cylinder);
}

PoleTarget::PoleTarget(glm::mat4 MT, SceneGeometry* cylinder): SceneObject(MT) {
	initPole(cylinder);
}

PoleTarget::~PoleTarget() {
	delete pole;
}
`
void PoleTarget::initPole(SceneGeometry* cylinder) {
	pole = new SceneTransform(glm::mat4(1));
	pole->addChild(cylinder);
	this_object->addChild(pole);
}

void PoleTarget::drawObject(GLuint shaderProgram, glm::mat4 C) {
	this_object->draw(shaderProgram, C);
}