#ifndef _LIGHTSRC_H_
#define _LIGHTSRC_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include "Object.h"
#include "PointCloud.h"

class LightSrc : public Object
{

private:
	GLuint vao, vbo[2], ebo;
	PointsObj getPoints(std::string filename);

public:
	glm::vec3 lightPos;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	std::vector<glm::vec3> points, normals;
	std::vector<glm::ivec3> indices;
	LightSrc(std::string objFilename, glm::vec3 startingPos, glm::vec3 color);
	~LightSrc();

	void draw();
	void update();

	void move(double x, double y, double z);
	void rotate(float deg, glm::vec3 rot_axis);
};

#endif