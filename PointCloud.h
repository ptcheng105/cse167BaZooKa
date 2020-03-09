#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

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
#include "Material.h"

struct PointsObj
{
	std::vector<glm::vec3> v;
	std::vector<glm::vec3> vn;
	std::vector<glm::ivec3> f;
	float x_max, x_min, y_max, y_min, z_max, z_min;
};

struct PointsObj2 {
	std::vector < glm::vec3 > out_vertices;
	std::vector < glm::vec2 > out_uvs;
	std::vector < glm::vec3 > out_normals;
};

class PointCloud : public Object
{

private:
	float rev_x, rev_y, rev_z, rev_scale;
	float rev_deg;
	char rotation;
	GLuint vao, vbo[2], ebo;
	
	PointsObj getPoints(std::string filename);
	
public:
	Material* mat;

	std::vector<glm::vec3> points, normals;
	std::vector<glm::ivec3> indices;
	GLfloat pointSize;
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();

	void updatePointSize(GLfloat size);
	void move(double x, double y, double z);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void moveIn();
	void moveOut();
	void scaleUp();
	void scaleDown();
	void resetPosition();
	void resetOriScale();
	void spin(float deg);
	void rotate(float deg, glm::vec3 rot_axis);
};

#endif

