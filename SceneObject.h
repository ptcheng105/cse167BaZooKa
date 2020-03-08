#ifndef _SCENEOBJECT_H_
#define _SCENEOBJECT_H_
#include "SceneNode.h"
#include "SceneTransform.h"
#include "SceneGeometry.h"
#include <glm/gtc/type_ptr.hpp>

class SceneObject
{
protected:
	SceneTransform * world2object;
	SceneTransform * this_object;
	glm::vec4 world_position = glm::vec4(0,0,0,1);
	glm::vec3 velocity = glm::vec3(0,0,0);

	//hitbox
	glm::mat4 hitbox_model;
	std::vector<glm::vec3> hitbox_vertices;
	glm::vec3 hitbox_half_dimension;
	GLuint hitbox_vao;
	GLuint hitbox_vbos[2];

public:
	SceneObject(glm::mat4 MT, SceneTransform* object, glm::vec3 hitbox_half_dimension);
	~SceneObject();

	void setObjVelocity(glm::vec3 vec);
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update();
	void objUpdate(glm::mat4 C);
	void isCollidedWith(SceneObject* targetObj);
	void collided();
};

#endif