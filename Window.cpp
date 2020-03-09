#include "Window.h"
#include <math.h> 
#include<algorithm> 
#include <iostream>

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
namespace
{
	int width, height;
	std::string windowTitle("BaZooKa!");

	// Boolean switches
	bool glRasterize = true;
	bool lightMode = false;
	GLint lightModeNum = 1;
	int controlModeNum = 1; // 1 = control model, 2 = control point light
	
	SkyBox* skybox;
	//word transform
	SceneTransform* world;

	//basic scene geometry
	SceneGeometry* cylinder, * cone, * sphere;

	//rocket
	Rocket * rocket1;
	SceneTransform *launcher;
	//SceneGeometry2* launcherGeometry;


	// target
	SceneObject* test_obj1, *test_obj2;

	//Identity matrix
	const glm::mat4 IM = glm::mat4(1.0f);
	glm::vec3 eye(0, 0, 15); // Camera position.
	glm::vec3 center(0, 0, 0); // The point we are looking at.
	glm::vec3 up(0, 1, 0); // The up direction of the camera.
	float fovy = 60;
	float near = 1;
	float far = 1000;
	glm::mat4 projection; // Projection matrix.

	GLuint program; // The shader program id.
	GLuint colorProgram, skyBoxProgram;

	GLuint lightPosLoc; //location of light pos in shader
	GLuint programQuad;

	//for mouse event
	double mouse_x, mouse_y;
	glm::vec3 last_trackball_point;
	bool dragging = false;
	bool rotating = false;

	// for camera controls
	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 15.0f);
	glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	glm::vec3 direction;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX = 960, lastY = 540;

	bool firstMouse = true;

	GLuint modelProgram;

	float camTime = 0.0;

};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	//load sky box shader
	skyBoxProgram = LoadShaders("shaders/skyBoxShader.vert", "shaders/skyBoxShader.frag");

	//load curve shader
	colorProgram = LoadShaders("shaders/colorShader.vert", "shaders/colorShader.frag");

	modelProgram = LoadShaders("shaders/modelshader.vert", "shaders/modelshader.frag");
	// Check the shader programs.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	if (!skyBoxProgram)
	{
		std::cerr << "Failed to initialize sky box program" << std::endl;
		//return false;
	}
	if (!colorProgram)
	{
		std::cerr << "Failed to initialize color program" << std::endl;
		//return false;
	}

	if (!modelProgram) {
		std::cerr << "Failed to initialize model program" << std::endl;
		//return false;
	}

	// Activate the shader program.
	glUseProgram(program);

	return true;
}

bool Window::initializeObjects()
{
	// create all the geometry in geometry library
	cylinder = new SceneGeometry("body_s.obj", 1, colorProgram);
	cone = new SceneGeometry("cone.obj", 2, colorProgram);
	sphere = new SceneGeometry("sphere.obj", 2, colorProgram);
	// create skybox
	skybox = new SkyBox();

	//rocket
	rocket1 = new Rocket(glm::vec3(0, 0, 0), cylinder, cone, sphere, colorProgram);
	//rocket1->velocity = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 1)) * glm::vec4(glm::vec3(0, 0.1, 0),1);
	//test object
	test_obj1 = new SceneObject(glm::vec3(8, 0, 0), colorProgram);
	//test_obj1->velocity = glm::vec3(-0.005, 0, 0);


	//launcherGeometry = new SceneGeometry2("rocketlauncher.obj");
	//launcherGeometry->genTexture("gun_D.jpg");

	//launcher = new SceneTransform(glm::scale(glm::mat4(1.0f), glm::vec3(.09f, .09f, .09f)) * glm::rotate(glm::mat4(1.0f), glm::radians(182.0f), glm::vec3(0, 1, 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(-17.0f, 3.0f, 25.0f )));
	//launcher->addChild(launcherGeometry);
	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cylinder;
	delete cone;
	delete sphere;
	delete skybox;
	delete rocket1;
	delete test_obj1, test_obj2;

	glDeleteProgram(program);
	glDeleteProgram(skyBoxProgram);
	glDeleteProgram(colorProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
#endif
	width = w;
	height = h;

	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{
	// Perform any updates as necessary.
	test_obj1->idleUpdate();
	rocket1->idleUpdate();
	//rocket1->rotateObj(-0.5f, glm::vec3(0, 0, 1));

	//check collision
	if (rocket1->isCollidedWith(test_obj1)) {
		rocket1->resolveCollision(true);
		test_obj1->resolveCollision(true);
	}
	else {
		rocket1->resolveCollision(false);
		test_obj1->resolveCollision(false);
	}

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}

void Window::displayCallback(GLFWwindow* window)
{
	// update view
	view = glm::lookAt(camPos, camPos + camFront, camUp);

	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthRange(0.0, 2.0);

	//sky box	
	glDepthMask(GL_FALSE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glUseProgram(skyBoxProgram);
	// ... set view and projection matrix
	glUniformMatrix4fv(glGetUniformLocation(skyBoxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(skyBoxProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	skybox->draw();
	glDepthMask(GL_TRUE);
	glDisable(GL_CULL_FACE);

	//draw test obj
	test_obj1->drawObject(program, projection, view);

	glUseProgram(modelProgram);
	glUniformMatrix4fv(glGetUniformLocation(modelProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(modelProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	//launcher->draw(modelProgram, glm::mat4(1.0f));

	//draw rocket
	rocket1->drawObject(program, projection, view);

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

glm::vec3 Window::trackBallMapping(GLFWwindow* window, double pointx, double pointy) {
	glm::vec3 ret_vec;
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float d;
	ret_vec.x = (2 * pointx - width) / width;
	ret_vec.y = (height - 2 * pointy) / height;
	ret_vec.z = 0;
	d = glm::length(ret_vec);
	d = (d < 1.0) ? d : 1.0;
	ret_vec.z = std::sqrt(1.001 - (d*d));
	glm::normalize(ret_vec);
	return ret_vec;
}


void Window::mouseMovementCallback(GLFWwindow* window, double xpos, double ypos) {
	if (dragging) {
		double movement_x = xpos - mouse_x;
		double movement_y = ypos - mouse_y;
		mouse_x = xpos;
		mouse_y = ypos;
	}else if (rotating) {
		glm::vec3 direction;
		float pixel_diff;
		float rot_angle, zoom_factor;
		glm::vec3 curPoint;
		curPoint = trackBallMapping(window, xpos, ypos);
		direction = curPoint - last_trackball_point;
		float velocity = glm::length(direction);
		if (velocity > 0.0001) {
			glm::vec3 rotAxis;
			rotAxis = glm::cross(last_trackball_point, curPoint);
			rot_angle = velocity * 50;// here is a constant to change angle
			if (controlModeNum == 1) {
				//view = glm::rotate(glm::mat4(1.0f), glm::radians(rot_angle), rotAxis) * view;
			}
			
			
		}
		last_trackball_point = curPoint;
	}
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	// for first person camera
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));


	camFront = glm::normalize(direction);
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		mouse_x = xpos;
		mouse_y = ypos;
		dragging = true;
		rotating = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		dragging = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		last_trackball_point = trackBallMapping(window, xpos, ypos);

		rotating = true;
		dragging = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		rotating = false;
	}
		
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (controlModeNum == 1) {
		//view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, (float)yoffset)) * view;
	}
	else {
		float scale = (yoffset > 0) ? 1.2: 0.8;	}
	
}

glm::mat4 Window::scaleRotateTranslate(glm::vec3 scale_vec, float deg, glm::vec3 rotAxis, glm::vec3 trans_vec) {
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), scale_vec);
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotAxis);
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), trans_vec);
	return  (trans * rot * scale);
}

glm::mat4 Window::translateRotateTranslate(float deg, glm::vec3 rotAxis, glm::vec3 trans_vec) {
	glm::mat4 trans1 = glm::translate(glm::mat4(1.0f), trans_vec);
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rotAxis);
	glm::mat4 trans2 = glm::translate(glm::mat4(1.0f), -trans_vec);
	return  (trans2 * rot * trans1);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	float camSpeed = 200.5f * deltaTime;
	/*
	 * TODO: Section 4: Modify below to add your key callbacks.
	 */

	 // Check for a key press.
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		// Uppercase key presses (shift held down + key press)
		if (mods == GLFW_MOD_SHIFT) {
			switch (key) {
			default:
				break;
			}
		}
		else {
			// Deals with lowercase key presses
			switch (key)
			{
			case GLFW_KEY_W:
				camPos += camSpeed * camFront;
				//launcher->update(glm::translate(glm::mat4(1.0f), camSpeed * camFront));
				break;
			case GLFW_KEY_S:
				camPos -= camSpeed * camFront;
				//launcher->update(glm::translate(glm::mat4(1.0f), -camSpeed * camFront));
				break;
			case GLFW_KEY_A:
				camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
				//launcher->update(glm::translate(glm::mat4(1.0f), -glm::normalize(glm::cross(camFront, camUp)) * camSpeed));
				break;
			case GLFW_KEY_D:
				camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
				//launcher->update(glm::translate(glm::mat4(1.0f), glm::normalize(glm::cross(camFront, camUp)) * camSpeed));
				break;
			case GLFW_KEY_C:
				controlModeNum = 1;
				break;
			case GLFW_KEY_X:
				controlModeNum = 2;
				break;
			case GLFW_KEY_ESCAPE:
				// Close the window. This causes the program to also terminate.
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			default:
				break;
			}
		}	
	}
}
