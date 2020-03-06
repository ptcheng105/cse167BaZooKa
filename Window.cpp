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
	std::string windowTitle("GLFW Starter Project");

	//scene graph
	SceneTransform *Robot;
	SceneTransform* RobotArmy;
	// Boolean switches
	bool glRasterize = true;
	bool lightMode = false;
	GLint lightModeNum = 1;
	int controlModeNum = 1; // 1 = control model, 2 = control point light

	Cube* cube;
	PointCloud* cubePoints, *bunnyPoints, *dragonPoints, *bearPoints, *currentObj;
	LightSrc* lamp;
	directLightSrc* dirlight;
	RasterizerQuad* quad;  // Object textured with your rasterization results
	
	SkyBox* skybox;
	//word transform
	SceneTransform* world;
	//robot geo and transform
	SceneGeometry* BodyGeometry, * LimbGeometry, * HeadGeometry, * EyeGeometry, * AntennaGeometry;
	SceneTransform* robotbody, * robothead, * robotLeye, * robotReye, * robotLantenna, * robotRantenna,
		* robotLarm, * robotRarm, * robotLleg, * robotRleg;
	SceneTransform* robotArmyRotationLayer;

	//robot animation
	float swingDeg = 0;
	float swingDegPerFrame = -0.5;

	//rocket
	SceneGeometry* cylinder, * cone, *sphere;
	SceneTransform* rocketHead, * rocketBody, * rocket;

	//curve
	SceneTransform *curvepath;
	BezierCurveGeometry *curve1, * curve2, *curve3, *curve4, *curve5;

	//curve animation
	std::vector<glm::vec3> all_curve_point;
	int num_of_total_curve_point, current_step=0, step_per_frame=1;
	glm::vec3 last_curve_point = glm::vec3(0, 0, 0);
	int delay_army_movement_counter = 0;
	int delay_army_movement_frames = 5;

	glm::vec3 eye(0, 0, 20); // Camera position.
	glm::vec3 center(0, 0, 0); // The point we are looking at.
	glm::vec3 up(0, 1, 0); // The up direction of the camera.
	float fovy = 60;
	float near = 1;
	float far = 1000;
	glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
	glm::mat4 projection; // Projection matrix.

	GLuint program; // The shader program id.
	GLuint projectionLoc; // Location of projection in shader.
	GLuint curveProgram, skyBoxProgram; // lighting shader program.
	GLuint viewLoc; // Location of view in shader.
	GLuint modelLoc; // Location of model in shader.
	GLuint colorLoc; // Location of color in shader.

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
	curveProgram = LoadShaders("shaders/curve.vert", "shaders/curve.frag");

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
	if (!curveProgram)
	{
		std::cerr << "Failed to initialize curve program" << std::endl;
		//return false;
	}


	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");

	return true;
}

bool Window::initializeObjects()
{
	// create skybox
	skybox = new SkyBox();


	// create cylinder and cone
	cylinder = new SceneGeometry("body_s.obj", 1);
	cone = new SceneGeometry("cone.obj", 2);
	sphere = new SceneGeometry("sphere.obj", 2);

	rocketHead = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f)));
	rocketHead->addChild(cone);

	rocketBody = new SceneTransform(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f)));
	rocketBody->addChild(cylinder);

	SceneTransform *jet_flame = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 0.0f))* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1,0,0)));
	SceneTransform* jet_flame_base = new SceneTransform(scaleRotateTranslate(glm::vec3(1.1, 1.1, 1.1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, -1.5, 0)));
	SceneTransform* jet_flame_spike_1 = new SceneTransform(scaleRotateTranslate(glm::vec3(1, 1, 1), 0.0f, glm::vec3(1,0,0), glm::vec3(0,0,0)) * translateRotateTranslate(-50.0f, glm::vec3(1, 0, 0), glm::vec3(0, 1.5, 0)));
	SceneTransform* jet_flame_spike_2 = new SceneTransform(scaleRotateTranslate(glm::vec3(1, 1, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)) * translateRotateTranslate(-50.0f, glm::vec3(-1, 0, -1), glm::vec3(0, 1.5, 0)));
	SceneTransform* jet_flame_spike_3 = new SceneTransform(scaleRotateTranslate(glm::vec3(1, 1, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)) * translateRotateTranslate(-50.0f, glm::vec3(-1, 0, 1), glm::vec3(0, 1.5, 0)));
	SceneTransform* jet_flame_spike_4 = new SceneTransform(scaleRotateTranslate(glm::vec3(1, 7.0f, 1), 0.0f, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0)));
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


	rocket = new SceneTransform(glm::rotate(glm::mat4(1.0f), glm::radians((float)90.0), glm::vec3(1,0,0)));
	//rocket = new SceneTransform(glm::mat4(1));
	rocket->addChild(rocketHead);
	rocket->addChild(rocketBody);
	rocket->addChild(jet_flame);
	/*
	//create curve
	curvepath = new SceneTransform(glm::mat4(1));
	float num_of_points_on_curve= 150.0f;
	curve1 = new BezierCurveGeometry(glm::vec3(0, 0, 0), glm::vec3(0, 0, 20), glm::vec3(0, 20, 20), glm::vec3(0, 20, 40), num_of_points_on_curve);
	curve2 = new BezierCurveGeometry(glm::vec3(0, 20, 40), glm::vec3(0, 20, 60), glm::vec3(0, 0, 60), glm::vec3(0, 0, 80), num_of_points_on_curve);
	curve3 = new BezierCurveGeometry(glm::vec3(0, 0, 80), glm::vec3(0, 0, 90), glm::vec3(40, 0, 90), glm::vec3(40, 0, 80), num_of_points_on_curve);
	curve4 = new BezierCurveGeometry(glm::vec3(40, 0, 80), glm::vec3(40, 0, 40), glm::vec3(80, 0, 80), glm::vec3(40, 0, 20), num_of_points_on_curve);
	curve5 = new BezierCurveGeometry(glm::vec3(40, 0, 20), glm::vec3(0, 0, -40), glm::vec3(0, 0, -20), glm::vec3(0, 0, 0), num_of_points_on_curve);
	curvepath->addChild(curve1);
	curvepath->addChild(curve2);
	curvepath->addChild(curve3);
	curvepath->addChild(curve4);
	curvepath->addChild(curve5);

	//store curve point
	all_curve_point.insert(all_curve_point.end(), curve1->curve_points.begin(), curve1->curve_points.end());
	all_curve_point.insert(all_curve_point.end(), curve2->curve_points.begin(), curve2->curve_points.end());
	all_curve_point.insert(all_curve_point.end(), curve3->curve_points.begin(), curve3->curve_points.end());
	all_curve_point.insert(all_curve_point.end(), curve4->curve_points.begin(), curve4->curve_points.end());
	all_curve_point.insert(all_curve_point.end(), curve5->curve_points.begin(), curve5->curve_points.end());
	num_of_total_curve_point = all_curve_point.size();

	// create robot geometry
	LimbGeometry = new SceneGeometry("limb_s.obj");
	BodyGeometry = new SceneGeometry("body_s.obj");
	HeadGeometry = new SceneGeometry("head_s.obj");
	EyeGeometry = new SceneGeometry("eyeball_s.obj");
	AntennaGeometry = new SceneGeometry("antenna_s.obj");

	// create robot parts (transform)
	Robot = new SceneTransform(glm::mat4(1.0f));

	// robot head and body
	robothead = new SceneTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.2f, 0.0f)));
	robotbody = new SceneTransform(glm::mat4(1));
	Robot->addChild(robothead);
	Robot->addChild(robotbody);
	robothead->addChild(HeadGeometry);
	robotbody->addChild(BodyGeometry);

	//robot eyes
	robotLeye = new SceneTransform(scaleRotateTranslate(glm::vec3(1.2, 1.2, 1.2), 0.0, glm::vec3(0, 0, 1), glm::vec3(-0.5f, 1.5f, 0.8f)));
	robotReye = new SceneTransform(scaleRotateTranslate(glm::vec3(1.2, 1.2, 1.2), 0.0, glm::vec3(0, 0, 1), glm::vec3(0.5f, 1.5f, 0.8f)));
	Robot->addChild(robotLeye);
	Robot->addChild(robotReye);
	robotLeye->addChild(EyeGeometry);
	robotReye->addChild(EyeGeometry);

	//robot antenna
	robotLantenna = new SceneTransform(scaleRotateTranslate(glm::vec3(0.2, 0.2, 0.2), 30.0, glm::vec3(0,0,1), glm::vec3(-0.8,1.8,0)));
	robotRantenna = new SceneTransform(scaleRotateTranslate(glm::vec3(0.2, 0.2, 0.2), -30.0, glm::vec3(0, 0, 1), glm::vec3(0.8,1.8,0)));
	Robot->addChild(robotLantenna);
	Robot->addChild(robotRantenna);
	robotLantenna->addChild(AntennaGeometry);
	robotRantenna->addChild(AntennaGeometry);

	//robot arm
	robotLarm = new SceneTransform(translateRotateTranslate(-22.5, glm::vec3(1, 0, 0), glm::vec3(0, -0.8, 0)) * scaleRotateTranslate(glm::vec3(1.4, 1.4, 1.4), 0.0, glm::vec3(0, 0, 1), glm::vec3(-1.3f, 0.2f, 0.0f)));
	robotRarm = new SceneTransform(translateRotateTranslate(-22.5, glm::vec3(1, 0, 0), glm::vec3(0, -0.8, 0)) * scaleRotateTranslate(glm::vec3(1.4, 1.4, 1.4), 0.0, glm::vec3(0, 0, 1), glm::vec3(1.3f, 0.2f, 0.0f)));
	Robot->addChild(robotLarm);
	Robot->addChild(robotRarm);
	robotLarm->addChild(LimbGeometry);
	robotRarm->addChild(LimbGeometry);

	//robot leg
	robotLleg = new SceneTransform(scaleRotateTranslate(glm::vec3(1.4, 1.4, 1.4), 0.0, glm::vec3(0, 0, 1), glm::vec3(-0.5f, -1.4f, 0.0f)));
	robotRleg = new SceneTransform(scaleRotateTranslate(glm::vec3(1.4, 1.4, 1.4), 0.0, glm::vec3(0, 0, 1), glm::vec3(0.5f, -1.4f, 0.0f)));
	Robot->addChild(robotLleg);
	Robot->addChild(robotRleg);
	robotLleg->addChild(LimbGeometry);
	robotRleg->addChild(LimbGeometry);

	//army of robot
	RobotArmy = new SceneTransform(glm::mat4(1.0f));
	robotArmyRotationLayer = new SceneTransform(glm::mat4(1.0f));
	RobotArmy->addChild(robotArmyRotationLayer);
	for (int i = -2; i < 3; i++) {
		for (int j = -2; j < 3; j++) {
			SceneTransform* oneRobot = new SceneTransform(glm::translate(glm::mat4(1), glm::vec3(i * 3, 0 , j * 3)));
			oneRobot->addChild(Robot);
			robotArmyRotationLayer->addChild(oneRobot);
		}
	}
	*/

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
	delete cubePoints;
	delete quad;
	delete bunnyPoints;
	delete dragonPoints;
	delete bearPoints;
	delete lamp;

	// Delete the shader programs.
	glDeleteProgram(program);
	glDeleteProgram(programQuad);
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
	
	// Initialize the quad that will be textured with your image
	// The quad must be made with the window
	quad = new RasterizerQuad(width, height);

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

	// Resize our CPU rasterizer's pixel buffer and zbuffer
	quad->updateBufSiz(width, height);

	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{
	// Perform any updates as necessary. 

	/*
	// walk animation
	//if swing deg reaches -90 or +45, reverse
	if (swingDeg >= 67.5 || swingDeg <= -67.5) {
		swingDegPerFrame = -1 * swingDegPerFrame;
	}
	swingDeg += swingDegPerFrame;

	// update transform
	robotLarm->update(translateRotateTranslate(swingDegPerFrame, glm::vec3(1, 0, 0), glm::vec3(0, -0.8, 0)));
	robotRarm->update(translateRotateTranslate(-swingDegPerFrame, glm::vec3(1, 0, 0), glm::vec3(0, -0.8, 0)));
	robotLleg->update(translateRotateTranslate(-swingDegPerFrame, glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)));
	robotRleg->update(translateRotateTranslate(swingDegPerFrame, glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)));

	// move robot army
	if (delay_army_movement_counter >= delay_army_movement_frames) {
		glm::vec3 current_curve_point = all_curve_point[current_step];
		glm::vec3 translation = current_curve_point - last_curve_point; //calculate translate
		RobotArmy->update(glm::translate(glm::mat4(1), translation));
		last_curve_point = current_curve_point;
		current_step = (current_step + 1) % num_of_total_curve_point;

		//increment delay counter
		delay_army_movement_counter = 0;
	}
	else {
		delay_army_movement_counter++;
	}
	*/

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

	/*draw rocket*/
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	rocket->draw(program, glm::mat4(1));

	/*
	//building scene graph
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	RobotArmy->draw(program, glm::mat4(1));

	//build curve
	glUseProgram(curveProgram);
	glUniformMatrix4fv(glGetUniformLocation(curveProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(curveProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	curvepath->draw(curveProgram, glm::mat4(1));
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
				view = glm::lookAt(eye, center, up);
			}
			else {
				robotArmyRotationLayer->update(glm::rotate(glm::mat4(1.0f), glm::radians(rot_angle), rotAxis));
			}
			
			
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
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, (float)yoffset)) * view;
	}
	else {
		float scale = (yoffset > 0) ? 1.2: 0.8;
		robotArmyRotationLayer->update(glm::scale(glm::mat4(1), glm::vec3(scale)));
	}
	
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
