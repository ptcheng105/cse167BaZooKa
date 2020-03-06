#include "lightSrc.h"

LightSrc::LightSrc(std::string objFilename, glm::vec3 startingPos, glm::vec3 light_color){
	PointsObj pointObj = getPoints(objFilename);
	points = pointObj.v;
	normals = pointObj.vn;
	indices = pointObj.f;

	model = glm::mat4(1);

	color = light_color;
	ambient = light_color;
	diffuse = light_color;
	specular = light_color;

	constant = 1.0;
	linear = 0.22;
	quadratic = 0.0;

	//set it to a starting point and scale it
	lightPos = startingPos;
	model = glm::translate(glm::mat4(1.0f), lightPos) * model;
	model = glm::scale(model, glm::vec3(0.2f));


	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);

	// Bind to the VAO.
	// This tells OpenGL which data it should be paying attention to
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// HW2: gen EBO for triangles
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

LightSrc::~LightSrc()
{
	// Delete the VBO and the VAO.
	// Failure to delete your VAOs, VBOs and other data given to OpenGL
	// is dangerous and may slow your program and cause memory leaks
	glDeleteBuffers(1, &vbo[0]);
	glDeleteBuffers(1, &vbo[1]);
	glDeleteVertexArrays(1, &vao);
}

PointsObj LightSrc::getPoints(std::string filename) {
	float x, y, z, vnx, vny, vnz;
	float r, g, b;
	char buffer[256];
	int c1, c2, read_c = 1;
	int f1, f2, f3;

	FILE* fp;
	PointsObj retObj;

	fp = fopen(filename.c_str(), "r");

	c1 = fgetc(fp);
	while (c1 != EOF) {
		c2 = fgetc(fp);
		if (c1 == 'v' && c2 == ' ') {
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			if (x > retObj.x_max) { retObj.x_max = x; };
			if (x < retObj.x_min) { retObj.x_min = x; };
			if (x > retObj.y_max) { retObj.y_max = y; };
			if (x < retObj.y_min) { retObj.y_min = y; };
			if (x > retObj.z_max) { retObj.z_max = z; };
			if (x < retObj.z_min) { retObj.z_min = z; };
			retObj.v.push_back(glm::vec3(x, y, z));
		}
		else if (c1 == 'v' && c2 == 'n') {
			fscanf(fp, "%f %f %f", &vnx, &vny, &vnz);
			retObj.vn.push_back(glm::vec3(vnx, vny, vnz));
		}
		else if (c1 == 'f' && c2 == ' ') {
			fscanf(fp, "%d//%d %d//%d %d//%d", &f1, &f1, &f2, &f2, &f3, &f3);
			retObj.f.push_back(glm::ivec3(f1 - 1, f2 - 1, f3 - 1));
		}

		c1 = c2;
	}
	return retObj;
}

void LightSrc::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);

	//draw triangles
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void LightSrc::update()
{
	// Spin the cube by 1 degree.
	//spin(0.05f);
	/*
	 * TODO: Section 3: Modify this function to spin the dragon and bunny about
	 * different axes. Look at the spin function for an idea
	 */
}

void LightSrc::move(double x, double y, double z) {
	model = glm::translate(glm::mat4(1.0f), glm::vec3((float)x, (float)y, (float)z))* model;
	lightPos = lightPos + glm::vec3((float)x, (float)y, (float)z);
}

void LightSrc::rotate(float deg, glm::vec3 rot_axis) {
	model = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rot_axis) * model;
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rot_axis);
	lightPos = glm::vec3(rotMat * glm::vec4(lightPos, 1.0));
}