#include "destroyTarget.h"
destroyTarget::destroyTarget(glm::vec3 position_in_world,GLuint colorProg, GLuint hitbox_prog) : SceneObject(position_in_world, hitbox_prog) {
	// create all the geometry in geometry library
	SceneGeometry* cylinder = new SceneGeometry("body_s.obj", 1, colorProg);
	SceneGeometry* base_cy = new SceneGeometry("body_s.obj", 1, colorProg);

	//set color here
	cylinder->geo_color = glm::vec3(0.3, 0.3, 0.3);
	base_cy->geo_color = glm::vec3(0, 0.3, 0);
	
	//build target
	glm::mat4 pipesize = glm::scale(glm::mat4(1), glm::vec3(0.5, 2, 0.5));
	canon_pipe = new SceneTransform(glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1,0,0))*pipesize);
	canon_pipe->addChild(cylinder);


	pipe1 = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f)));
	pipe2 = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	pipe3 = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)));
	pipe1->addChild(canon_pipe);
	pipe2->addChild(canon_pipe);
	pipe3->addChild(canon_pipe);
	pipes  = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 3.5f)));
	pipes->addChild(pipe1);
	pipes->addChild(pipe2);
	pipes->addChild(pipe3);

	glm::mat4 basesize = glm::scale(glm::mat4(1), glm::vec3(3, 2, 3));
	canon_base = new SceneTransform(glm::mat4(1)* basesize);
	canon_base->addChild(base_cy);

	this_object->addChild(canon_base);
	this_object->addChild(pipes);

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
	delete(canon_base);
	delete(canon_pipe);
	delete(pipes);
	delete(pipe1);
	delete(pipe2);
	delete(pipe3);
}