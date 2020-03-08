#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class SceneNode
{
public:
	SceneNode() {};
	virtual ~SceneNode() {};

	virtual void draw(GLuint shaderProgram, glm::mat4 projection, glm::mat4 view, glm::mat4 C) = 0;
	virtual void update(glm::mat4 C) = 0;
};

#endif