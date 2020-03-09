#include "SceneObject.h"

SceneObject::SceneObject(glm::vec3 position_in_world, GLuint hitbox_prog){
	translateTM = glm::translate(glm::mat4(1), position_in_world);
	rot_scaleTM = glm::mat4(1);
	this_object = new SceneTransform(glm::mat4(1));
	this->position_in_world = glm::vec4(position_in_world,1);

	//hitbox;
	hitbox_program = hitbox_prog;
	hitbox_model = translateTM * rot_scaleTM;
	hitbox_vertices =
	{
		glm::vec3(-1, 1, 1),
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1)
	};
	hitbox_indices =
	{
		// Front face.
		glm::ivec3(0, 1, 2),
		glm::ivec3(2, 3, 0),
		// Back face.
		glm::ivec3(7, 6, 5),
		glm::ivec3(5, 4, 7),
		// Right face.
		glm::ivec3(3, 2, 6),
		glm::ivec3(6, 7, 3),
		// Left face.
		glm::ivec3(4, 5, 1),
		glm::ivec3(1, 0, 4),
		// Top face.
		glm::ivec3(4, 0, 3),
		glm::ivec3(3, 7, 4),
		// Bottom face.
		glm::ivec3(1, 5, 6),
		glm::ivec3(6, 2, 1),
	};
	hitbox_half_dimension = glm::vec3(1, 1, 1);
	bindHitboxData();
	XYZMaxMin = { 1,-1,1,-1,1,-1 };

	//calculate min max on xaxis
	std::vector<glm::vec3> thisobj_corners = getHitboxCorners();
	SATtest(glm::vec3(1, 0, 0), thisobj_corners, xAxis_min, xAxis_max);
}	

SceneObject::~SceneObject()
{
	delete this_object;

	// Delete the VBOs and the VAO.
	glDeleteBuffers(2, hitbox_vbos);
	glDeleteVertexArrays(1, &hitbox_vao);
}

void SceneObject::bindHitboxData() {
	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &hitbox_vao);
	glGenBuffers(2, hitbox_vbos);
	// Bind to the VAO.
	glBindVertexArray(hitbox_vao);
	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, hitbox_vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * hitbox_vertices.size(),
		hitbox_vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	// Bind to the second VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hitbox_vbos[1]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * hitbox_indices.size(),
		hitbox_indices.data(), GL_STATIC_DRAW);
	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void SceneObject::drawObject(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view) {
	if (drawHitbox) {
		//draw hitbox
		glUseProgram(hitbox_program);
		glBindVertexArray(hitbox_vao);
		glUniformMatrix4fv(glGetUniformLocation(hitbox_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(hitbox_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(hitbox_program, "model"), 1, GL_FALSE, glm::value_ptr(hitbox_model));
		glUniform3fv(glGetUniformLocation(hitbox_program, "color"), 1, glm::value_ptr(hitbox_color));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(0);
	}
	this_object->draw(shaderProgram, projection, view, translateTM * rot_scaleTM);
}

void SceneObject::idleUpdate() {
	//update object with set physics
	velocity = velocity + acceleration;
	glm::mat4 changeinpos = glm::translate(glm::mat4(1.0f), velocity);
	position_in_world = changeinpos * position_in_world;
	translateTM = changeinpos * translateTM;

	//make sure to update hitbox model lastly
	hitbox_model = translateTM * rot_scaleTM;

	//calculate min max on xaxis
	std::vector<glm::vec3> thisobj_corners = getHitboxCorners();
	SATtest(glm::vec3(1,0,0), thisobj_corners, xAxis_min, xAxis_max);
}

bool SceneObject::isCollidedWith(SceneObject* targetObj) {
	//get objects hitbox corner in world
	std::vector<glm::vec3> thisobj_corners = getHitboxCorners();
	std::vector<glm::vec3> targetobj_corners = targetObj->getHitboxCorners();
	//test using the normal axis for this object
	std::vector<glm::vec3> uvw = {
		rot_scaleTM * glm::vec4(1,0,0,1),
		rot_scaleTM * glm::vec4(0, 1, 0, 1),
		rot_scaleTM * glm::vec4(0, 0, 1, 1)
	};
	for (int i = 0; i < uvw.size(); i++) {
		float shape1Min, shape1Max, shape2Min, shape2Max;
		SATtest(uvw[i], thisobj_corners, shape1Min, shape1Max);
		SATtest(uvw[i], targetobj_corners, shape2Min, shape2Max);
		if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max)) {
			return false;
		}
	}
	//if that succeed, test using the targetObj normal axis
	uvw = {
		targetObj->rot_scaleTM * glm::vec4(1,0,0,1),
		targetObj->rot_scaleTM * glm::vec4(0, 1, 0, 1),
		targetObj->rot_scaleTM * glm::vec4(0, 0, 1, 1)
	};
	for (int i = 0; i < uvw.size(); i++) {
		float shape1Min, shape1Max, shape2Min, shape2Max;
		SATtest(uvw[i], thisobj_corners, shape1Min, shape1Max);
		SATtest(uvw[i], targetobj_corners, shape2Min, shape2Max);
		if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max)) {
			return false;
		}
	}
	//all overlap means it is true in intersect

	return true;
}

void SceneObject::resolveCollision(bool collided) {
	if (collided) {
		hitbox_color = glm::vec3(1, 0, 0);//set hit box to red
		velocity = glm::vec3(0, 0, 0);
		destroyed = true;
	}
	else {
		hitbox_color = glm::vec3(1, 1, 1);//set to white
	}
	
}

void SceneObject::rotateObj(float deg, glm::vec3 rotAxis) {
	rot_scaleTM = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotAxis) * rot_scaleTM;
	hitbox_model = translateTM * rot_scaleTM;
}

void SceneObject::scaleObj(glm::vec3 scale) {
	rot_scaleTM = glm::scale(glm::mat4(1.0f), scale) * rot_scaleTM;
	hitbox_model = translateTM * rot_scaleTM;
}

void SceneObject::SATtest(const glm::vec3& axis, const std::vector<glm::vec3>& pointSet, float& minInAxis, float& maxInAxis) {
	minInAxis = FLT_MAX;
	maxInAxis = -FLT_MAX;
	for (int i = 0; i < pointSet.size(); i++)
	{
		// just dot it to get the min/max along this axis.
		float dotVal = glm::dot(pointSet[i],axis);
		if (dotVal < minInAxis)  minInAxis = dotVal;
		if (dotVal > maxInAxis)  maxInAxis = dotVal;
	}
}

std::vector<glm::vec3> SceneObject::getHitboxCorners() {
	std::vector<glm::vec3> transformed_vertices;

	for (int i = 0; i < hitbox_vertices.size(); i++) {
		transformed_vertices.push_back((rot_scaleTM * glm::vec4(hitbox_vertices[i], 1)) + position_in_world);
	}

	return transformed_vertices;
}

bool SceneObject::overlaps(float min1, float max1, float min2, float max2)
{
	return isBetweenOrdered(min2, min1, max1) || isBetweenOrdered(min1, min2, max2);
}

bool SceneObject::isBetweenOrdered(float val, float lowerBound, float upperBound) {
	return lowerBound <= val && val <= upperBound;
}