#ifndef _WATERTILE_H_
#define _WATERTILE_H_
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class WaterTile
{
private:
	GLuint waterProg;
	GLuint vao;
	GLuint vbos[2];
	std::vector<glm::vec3> vertices;

	glm::mat4 model;
	glm::vec3 waterColor;
	glm::vec3* camPos_ptr;
public:
	GLuint skyBoxTextureID;
	WaterTile(GLuint waterProgram, float toX, float toZ, GLuint skyboxtextureID, glm::vec3* campos_ptr);
	~WaterTile();

	void draw(glm::mat4 projection, glm::mat4 view);
};
#endif
