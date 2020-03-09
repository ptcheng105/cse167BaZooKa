#ifndef _SCENEOBJECT_H_
#define _SCENEOBJECT_H_
#include "SceneNode.h"
#include "SceneTransform.h"
#include "SceneGeometry.h"
#include <glm/gtc/type_ptr.hpp>
struct SATtest_return {
	float min;
	float max;
};
class SceneObject
{
public:
	float xAxis_min, xAxis_max;

	glm::mat4 translateTM;
	glm::mat4 rot_scaleTM;
	SceneTransform* this_object;

	//physics
	glm::vec4 position_in_world;
	glm::vec3 velocity = glm::vec3(0, 0, 0);
	glm::vec3 acceleration = glm::vec3(0, 0, 0);

	//hitbox
	GLuint hitbox_program;
	glm::mat4 hitbox_model;
	std::vector<glm::vec3> hitbox_vertices;
	std::vector<glm::ivec3> hitbox_indices;
	glm::vec3 hitbox_half_dimension;
	GLuint hitbox_vao;
	GLuint hitbox_vbos[2];
	bool drawHitbox = true;
	glm::vec3 hitbox_color = glm::vec3(1, 1, 1);
	std::vector<float> XYZMaxMin;
	bool destroyed = false;

	SceneObject(glm::vec3 position_in_world, GLuint hitbox_prog);
	~SceneObject();

	void bindHitboxData();

	void drawObject(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view);
	void idleUpdate();

	bool isCollidedWith(SceneObject* targetObj);
	void resolveCollision(bool collided);
	void rotateObj(float deg, glm::vec3 rotAxis);
	void scaleObj(glm::vec3 scale);
	void SATtest(const glm::vec3& axis, const std::vector<glm::vec3>& pointSet, float& minInAxis, float& maxInAxis);
	std::vector<glm::vec3> getHitboxCorners();
	bool overlaps(float min1, float max1, float min2, float max2);
	bool isBetweenOrdered(float val, float lowerBound, float upperBound);
	
};

#endif