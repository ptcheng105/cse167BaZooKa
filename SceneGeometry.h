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

public:
	glm::mat4 model;
	std::vector<glm::vec3> points, normals;
	std::vector<glm::ivec3> indices;

	SceneGeometry(std::string objFilename, int use_getPoint_number);
	~SceneGeometry();

	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 C);
};

#endif