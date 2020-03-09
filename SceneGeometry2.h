#pragma once
#ifndef _SCENEGEOMETRY2_H_
#define _SCENEGEOMETRY2_H_
#include "SceneNode.h"
#include <string>
#include <iostream>
#include <vector>
#include "PointCloud.h"

class SceneGeometry2 :
	public SceneNode
{
private:
	unsigned int texture;
	void getPoints(std::string filename);
	GLuint vao, vbo[2], ebo;

public:
	glm::mat4 model;
	std::vector<glm::vec3> vertices, normals, indeces;
	std::vector<glm::vec2> uvs;

	SceneGeometry2(std::string objFilename);
	void genTexture(std::string filename);
	~SceneGeometry2();

	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 C);
};

#endif