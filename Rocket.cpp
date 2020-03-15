#include "Rocket.h"
Rocket::Rocket(glm::vec3 position_in_world, SceneGeometry* cylinder, SceneGeometry* cone, SceneGeometry* sphere, GLuint hitbox_prog): SceneObject(position_in_world, hitbox_prog) {
	//set color here
	cone->geo_color = glm::vec3(0.8, 0, 0);
	sphere->geo_color = glm::vec3(0.8,0,0);
	cylinder->geo_color = glm::vec3(.5, .5, .5);
	//set hitbox_program to hit_box_prog
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

	this_object->addChild(rocketHead);
	this_object->addChild(rocketBody);
	this_object->addChild(jet_flame);

	//find out the max points in rocket
	XYZMaxMin = this_object->getXYZMaxMin(glm::mat4(1));

	//find object center
	float x_mid = (XYZMaxMin[0] + XYZMaxMin[1]) / 2;
	float y_mid = (XYZMaxMin[2] + XYZMaxMin[3]) / 2;
	float z_mid = (XYZMaxMin[4] + XYZMaxMin[5]) / 2;
	hitbox_half_dimension = glm::vec3((XYZMaxMin[0] - x_mid), (XYZMaxMin[2] - y_mid), (XYZMaxMin[4] - z_mid));

	//shift the combined model this object to the center of object space
	this_object->update(glm::translate(glm::mat4(1), glm::vec3(-x_mid, -y_mid, -z_mid)));

	//hit box
	hitbox_vertices = {
		glm::vec3(-hitbox_half_dimension.x, hitbox_half_dimension.y, hitbox_half_dimension.z),
		glm::vec3(-hitbox_half_dimension.x, -hitbox_half_dimension.y, hitbox_half_dimension.z),
		glm::vec3(hitbox_half_dimension.x, -hitbox_half_dimension.y, hitbox_half_dimension.z),
		glm::vec3(hitbox_half_dimension.x, hitbox_half_dimension.y, hitbox_half_dimension.z),
		glm::vec3(-hitbox_half_dimension.x, hitbox_half_dimension.y, -hitbox_half_dimension.z),
		glm::vec3(-hitbox_half_dimension.x, -hitbox_half_dimension.y, -hitbox_half_dimension.z),
		glm::vec3(hitbox_half_dimension.x, -hitbox_half_dimension.y, -hitbox_half_dimension.z),
		glm::vec3(hitbox_half_dimension.x, hitbox_half_dimension.y, -hitbox_half_dimension.z)
	};
	bindHitboxData();
	//calculate min max on xaxis
	std::vector<glm::vec3> thisobj_corners = getHitboxCorners();
	SATtest(glm::vec3(1, 0, 0), thisobj_corners, xAxis_min, xAxis_max);
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