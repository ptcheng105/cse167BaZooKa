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

	//hitbox
	glm::vec3 origin_in_world = glm::vec3(0, 0, 0);
	std::vector<glm::vec3> hitbox_vertices;
	GLuint hitbox_vao;
	GLuint hitbox_vbos[2];
	PointsObj pointObj;
public:
	bool drawHitbox = true;
	glm::vec3 hitbox_color = glm::vec3(1,1,1);
	GLuint hitbox_program;
	glm::vec3 hitbox_half_dimension;

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