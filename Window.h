#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"
#include "RasterizerQuad.h"
#include "lightSrc.h"
#include "directLightSrc.h"
#include "SkyBox.h"
#include "SceneNode.h"
#include "SceneTransform.h"
#include "SceneGeometry.h"
#include "SceneGeometry2.h"
#include "BezierCurveGeometry.h"
#include "SceneObject.h"
#include "Rocket.h"
class Window
{
public:
	// Initializes your shader program(s) and uniform variable locations
	static bool initializeProgram();
	// Initialize your OBJ objects here
	static bool initializeObjects();
	// Make sure to clean up any data you dynamically allocated here
	static void cleanUp();
	// Creates a GLFW window context
	static GLFWwindow* createWindow(int width, int height);
	// Is called whenever the window is resized
	static void resizeCallback(GLFWwindow* window, int width, int height);
	// Is called on idle.
	static void idleCallback();
	// This renders to the glfw window. Add draw calls here
	static void displayCallback(GLFWwindow*);
	// Add your key press event handling here
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	// handle mouse movement event
	static void mouseMovementCallback(GLFWwindow* window, double xpos, double ypos);
	// handle mouse button event
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	// scroll call back
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::vec3 trackBallMapping(GLFWwindow* window, double pointx, double pointy);
	//generate a transformation matrix
	static glm::mat4 scaleRotateTranslate(glm::vec3 scale_vec, float deg, glm::vec3 rotAxis, glm::vec3 trans_vec);
	static glm::mat4 translateRotateTranslate(float deg, glm::vec3 rotAxis, glm::vec3 trans_vec);
	static void generateAndShootRocket();
	static void insertionSort(std::vector<SceneObject*> &vec);
	static bool cmpSceneObj(SceneObject* obj1, SceneObject* obj2);
};

#endif
