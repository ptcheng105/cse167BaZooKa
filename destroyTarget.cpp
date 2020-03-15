#include "destroyTarget.h"
destroyTarget::destroyTarget(glm::vec3 position_in_world,GLuint colorProg, GLuint hitbox_prog) : SceneObject(position_in_world, hitbox_prog) {
	// create all the geometry in geometry library
	SceneGeometry* cylinder = new SceneGeometry("body_s.obj", 1, colorProg);
	SceneGeometry* cone = new SceneGeometry("cone.obj", 2, colorProg);

	//set color here
	cone->geo_color = glm::vec3(0.8, 0, 0);
	cylinder->geo_color = glm::vec3(0.8, 0.8, 0.8);

	//set hitbox_program to hit_box_prog
	rocketHead = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f)));
	rocketHead->addChild(cone);

	rocketBody = new SceneTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f)));
	rocketBody->addChild(cylinder);

	this_object->addChild(rocketHead);
	this_object->addChild(rocketBody);

	//scale this object
	this_object->update(glm::scale(glm::mat4(1), glm::vec3(6, 6, 6)));

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
destroyTarget::~destroyTarget() {
	delete(rocketHead);
	delete(rocketBody);
}