#include "SceneObject.h"

SceneObject::SceneObject(glm::mat4 MT, SceneTransform* object, glm::vec3 hitbox_half_dimension) {
	hitbox_model = MT;
	world2object = new SceneTransform(MT);
	this_object = object;
	world2object->addChild(this_object);
	world_position = MT * world_position;
	this->hitbox_half_dimension = hitbox_half_dimension;
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
	std::vector<glm::ivec3> hitbox_indices
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


SceneObject::~SceneObject()
{
	delete world2object;
	delete this_object;

	// Delete the VBOs and the VAO.
	glDeleteBuffers(2, hitbox_vbos);
	glDeleteVertexArrays(1, &hitbox_vao);
}

void SceneObject::setObjVelocity(glm::vec3 vec) {
	velocity = vec;
}

void SceneObject::draw(GLuint shaderProgram, glm::mat4 C) {
	//draw hitbox
	glBindVertexArray(hitbox_vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

void SceneObject::update() {
	//update world2object transform and world_position with velocity
	glm::mat4 T = glm::translate(glm::mat4(1.0f), velocity);
	world2object->update(T);
	world_position = T * world_position;
}

void SceneObject::objUpdate(glm::mat4 C) {
	this_object->update(C);
	this->update();
}

void SceneObject::isCollidedWith(SceneObject* targetObj) {

}
void SceneObject::collided() {

}