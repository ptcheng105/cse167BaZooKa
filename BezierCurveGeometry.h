#ifndef _BEZIERCURVEGEOMETRY_H_
#define _BEZIERCURVEGEOMETRY_H_
#include "SceneNode.h"
#include <vector>
#include <cmath>
#include "SceneNode.h"
class BezierCurveGeometry :
	public SceneNode
{
private:
	GLuint vao, vbo;
	glm::mat4 Gmat;
	glm::mat4 Bmat;
	glm::vec3 getpoint(float t);

public:
	std::vector<glm::vec3> curve_points;
	glm::mat4 model;

	BezierCurveGeometry(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float num_of_points);
	~BezierCurveGeometry();

	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 C);
};

#endif