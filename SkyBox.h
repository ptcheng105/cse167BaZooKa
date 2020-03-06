#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "stb_image.h"

#include "Object.h"

class SkyBox : public Object
{
private:
	GLuint vao;
	GLuint vbo;
	GLuint loadCubemap(std::vector<std::string> faces);
public:
	GLuint skyBoxTextureID;
	SkyBox();
	~SkyBox();

	void draw();
	void update();
};

#endif
