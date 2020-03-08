#include "Window.h"
#include <math.h> 
#include<algorithm> 

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
	SceneObject* rocket1;

	// target
	SceneObject* target;

	//Identity matrix
	const glm::mat4 IM = glm::mat4(1.0f);
	glm::vec3 eye(0, 0, 20); // Camera position.
	glm::vec3 center(0, 0, 0); // The point we are looking at.
	glm::vec3 up(0, 1, 0); // The up direction of the camera.
	float fovy = 60;
	float near = 1;
	float far = 1000;
	glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
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
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	//load sky box shader
	skyBoxProgram = LoadShaders("shaders/skyBoxShader.vert", "shaders/skyBoxShader.frag");

	//load curve shader
	colorProgram = LoadShaders("shaders/colorShader.vert", "shaders/colorShader.frag");

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

	/*
	// create Rocket
	SceneTransform* rocketHead, * rocketBody, * jet_flame,
		* jet_flame_base, * jet_flame_spike_1, * jet_flame_spike_2,
		* jet_flame_spike_3, * jet_flame_spike_4;
	rocketHead = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f)));
	rocketHead->addChild(cone);

	rocketBody = new SceneTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f)));
	rocketBody->addChild(cylinder);

	jet_flame = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0)));
	jet_flame_base = new SceneTransform(Window::scaleRotateTranslate(glm::vec3(1.1, 1.1, 1.1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, -1.5, 0)));
	jet_flame_spike_1 = new SceneTransform(Window::scaleRotateTranslate(glm::vec3(1, 1, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)) * Window::translateRotateTranslate(-50.0f, glm::vec3(1, 0, 0), glm::vec3(0, 1.5, 0)));
	jet_flame_spike_2 = new SceneTransform(Window::scaleRotateTranslate(glm::vec3(1, 1, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)) * Window::translateRotateTranslate(-50.0f, glm::vec3(-1, 0, -1), glm::vec3(0, 1.5, 0)));
	jet_flame_spike_3 = new SceneTransform(Window::scaleRotateTranslate(glm::vec3(1, 1, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)) * Window::translateRotateTranslate(-50.0f, glm::vec3(-1, 0, 1), glm::vec3(0, 1.5, 0)));
	jet_flame_spike_4 = new SceneTransform(Window::scaleRotateTranslate(glm::vec3(1, 7.0f, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)));
	jet_flame_base->addChild(sphere);
	jet_flame_spike_1->addChild(cone);
	jet_flame_spike_2->addChild(cone);
	jet_flame_spike_3->addChild(cone);
	jet_flame_spike_4->addChild(cone);
	jet_flame->addChild(jet_flame_base);
	jet_flame->addChild(jet_flame_spike_1);
	jet_flame->addChild(jet_flame_spike_2);
	jet_flame->addChild(jet_flame_spike_3);
	jet_flame->addChild(jet_flame_spike_4);
	SceneTransform* flatten_rocket = new SceneTransform(glm::rotate(glm::mat4(1.0f), glm::radians((float)-90.0), glm::vec3(1, 0, 0)));
	flatten_rocket->addChild(rocketHead);
	flatten_rocket->addChild(rocketBody);
	flatten_rocket->addChild(jet_flame);

	rocket1 = new SceneObject(IM, flatten_rocket, glm::vec3(5,5,5));
	rocket1->setObjVelocity(glm::vec3(0, 0, -0.001));

	// create Target
	SceneTransform* pole;
	pole = new SceneTransform(glm::scale(IM, glm::vec3(5,5,5)));
	pole->addChild(cylinder);
	target = new SceneObject(glm::translate(IM,glm::vec3(0,0,-30)) , pole, glm::vec3(5, 5, 5));
	*/

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
	delete target;

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
	
	// move target away
	cylinder->update(glm::translate(IM, glm::vec3(0,0.001,0)));

	//check collision
	std::cout << "collided: "<< cone->collidedWith(cylinder) << std::endl;
	if (cone->collidedWith(cylinder)) {
		cone->hitbox_color = glm::vec3(1, 0, 0);
		cylinder->hitbox_color = glm::vec3(1, 0, 0);
	}
	else {
		cone->hitbox_color = glm::vec3(1, 1, 0);
		cylinder->hitbox_color = glm::vec3(1, 1, 0);
	}
}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	cone->draw(program,projection,view,glm::mat4(1));
	cylinder->draw(program, projection, view, glm::mat4(1));

	/*
	//draw rocket
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	rocket1->draw(program, glm::mat4(1));

	//draw target
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	target->draw(program, glm::mat4(1));

	*/

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
		view = glm::translate(glm::mat4(1), glm::vec3(movement_x / 50, -movement_y / 50, 0)) * view;
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
				eye = glm::rotate(glm::mat4(1.0f), glm::radians(rot_angle), rotAxis) * glm::vec4(eye, 1.0f);
				up = glm::rotate(glm::mat4(1.0f), glm::radians(rot_angle), rotAxis) * glm::vec4(up, 1.0f);
				view = glm::lookAt(eye, center, up);
			}
			else {			}
			
			
		}
		last_trackball_point = curPoint;
	}

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
		eye.z = eye.z - yoffset;
		//view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, (float)yoffset)) * view;
		view = glm::lookAt(eye, center, up);
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
	/*
	 * TODO: Section 4: Modify below to add your key callbacks.
	 */

	 // Check for a key press.
	if (action == GLFW_PRESS)
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
			case GLFW_KEY_C:
				controlModeNum = 1;
				break;
			case GLFW_KEY_X:
				controlModeNum = 2;
				break;
			case GLFW_KEY_A:
				view = glm::translate(glm::mat4(1), glm::vec3(2, 0, 0)) * view;
				break;
			case GLFW_KEY_D:
				view = glm::translate(glm::mat4(1), glm::vec3(-2, 0, 0)) * view;
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
