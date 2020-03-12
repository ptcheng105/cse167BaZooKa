#ifndef _SCENEGEOMETRY_H_
#define _SCENEGEOMETRY_H_
#include "SceneNode.h"
#include <string>
#include <iostream>
#include <vector>
#include "PointCloud.h"

class SceneGeometry :
	public SceneNode	
{
private:
	PointsObj getPoints(std::string filename);
	PointsObj getPoints2(std::string filename);
	GLuint vao, vbo[2], ebo;
	PointsObj pointObj;

public:
	glm::vec3 geo_color = glm::vec3(1,1,1);
	glm::mat4 model = glm::mat4(1.0f);

	std::vector<glm::vec3> points, normals;
	std::vector<glm::ivec3> indices;

	SceneGeometry(std::string objFilename, int use_getPoint_number, GLuint hitbox_prog);
	~SceneGeometry();

	void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::mat4 C);
	void update(glm::mat4 C);

	std::vector<float> getXYZMaxMin(glm::mat4 C);
};

#endif