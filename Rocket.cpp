#include "Rocket.h"
Rocket::Rocket(SceneGeometry* cylinder, SceneGeometry* cone, SceneGeometry* sphere) {
	initRocket(cylinder, cone, sphere);
}

Rocket::Rocket(glm::mat4 MT, SceneGeometry* cylinder, SceneGeometry* cone, SceneGeometry* sphere): SceneObject(MT){
	initRocket(cylinder, cone, sphere);
}

void Rocket::initRocket(SceneGeometry* cylinder, SceneGeometry* cone, SceneGeometry* sphere) {
	//initialize the object scene here
	rocketHead = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f)));
	rocketHead->addChild(cone);

	rocketBody = new SceneTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f)));
	rocketBody->addChild(cylinder);

	jet_flame = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0)));
	jet_flame_base = new SceneTransform(Window::scaleRotateTranslate(glm::vec3(1.1, 1.1, 1.1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, -1.5, 0)));
	jet_flame_spike_1 = new SceneTransform(Window::scaleRotateTranslate(glm::vec3(1, 1, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)) * Window::translateRotateTranslate(-50.0f, glm::vec3(1, 0, 0), glm::vec3(0, 1.5, 0)));
	jet_flame_spike_2 = new SceneTransform(Window::scaleRotateTranslate(glm::vec3(1, 1, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)) * Window::translateRotateTranslate(-50.0f, glm::vec3(-1, 0, -1), glm::vec3(0, 1.5, 0)));
	jet_flame_spike_3 = new SceneTransform(Window::scaleRotateTranslate(glm::vec3(1, 1, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)) * Window::translateRotateTranslate(-50.0f, glm::vec3(-1, 0, 1), glm::vec3(0, 1.5, 0)));
	jet_flame_spike_4 = new SceneTransform(Window::scaleRotateTranslate(glm::vec3(1, 7.0f, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)));
	jet_flame_base->addChild(sphere);
	jet_flame_spike_1->addChild(cone);
	jet_flame_spike_2->addChild(cone);
	jet_flame_spike_3->addChild(cone);
	jet_flame_spike_4->addChild(cone);
	jet_flame->addChild(jet_flame_base);
	jet_flame->addChild(jet_flame_spike_1);
	jet_flame->addChild(jet_flame_spike_2);
	jet_flame->addChild(jet_flame_spike_3);
	jet_flame->addChild(jet_flame_spike_4);


	SceneTransform* flatten_rocket = new SceneTransform(glm::rotate(glm::mat4(1.0f), glm::radians((float)-90.0), glm::vec3(1, 0, 0)));
	//rocket = new SceneTransform(glm::mat4(1));
	flatten_rocket->addChild(rocketHead);
	flatten_rocket->addChild(rocketBody);
	flatten_rocket->addChild(jet_flame);

	this_object->addChild(flatten_rocket);
}

Rocket::~Rocket() {
	delete(rocketHead);
	delete(rocketBody);
	delete(jet_flame);
	delete(jet_flame_base);
	delete(jet_flame_spike_1);
	delete(jet_flame_spike_2);
	delete(jet_flame_spike_3);
	delete(jet_flame_spike_4);
}

void Rocket::drawObject(GLuint shaderProgram, glm::mat4 C) {
	this_object->draw(shaderProgram, C);
}

void Rocket::move() {
	this_object->update(glm::translate(glm::mat4(1.0f), velocity));
}