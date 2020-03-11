#ifndef _TERRAIN_GENERATOR_H_
#define _TERRAIN_GENERATOR_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "stb_image.h"
#include <iostream>

class TerrainGenerator {
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::ivec3> triIndeces;
	std::vector<glm::vec3> normals;
	GLuint verticesCount;
	GLuint vao, vbo[3], ebo, texture;
	glm::mat4 model;
public:
	TerrainGenerator(int maxDivisions, float maxSize, float maxHeight, std::string filename);
	~TerrainGenerator();
	void DiamondSquare(int row, int col, int size, float offset, int divisions);
	void genTexture(std::string filename);
	void setBuffers();
	void draw(GLuint program, glm::mat4 projection, glm::mat4 view, glm::mat4 C);
};

#endif