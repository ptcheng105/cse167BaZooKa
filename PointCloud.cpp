#include "PointCloud.h"
#include <iostream>
#include <algorithm>
#include <cmath> 

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize)
	: pointSize(pointSize)
{
	/*
	 * TODO: Section 2: Currently, all the points are hard coded below.
	 * Modify this to read points from an obj file.
	 * Don't forget to load in the object normals for normal coloring as well
	 */
	PointsObj pointObj = getPoints(objFilename);
	points = pointObj.v;
	normals = pointObj.vn;
	indices = pointObj.f;

	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen.
	 */

	 // Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	// scale model using the samllest side
	float lengthx = pointObj.x_max - pointObj.x_min;
	float lengthy = pointObj.y_max - pointObj.y_min;
	float lengthz = pointObj.z_max - pointObj.z_min;

	float lengthmax = std::max(lengthx, lengthy);
	lengthmax = std::max(lengthmax, lengthz);
	float factor = 2 / lengthmax;
	model = glm::scale(model, glm::vec3(factor, factor, factor));

	//shift the model to center of screen
	float x_mid = (pointObj.x_max + pointObj.x_min) / 2;
	float y_mid = (pointObj.y_max + pointObj.y_min) / 2;
	float z_mid = (pointObj.z_max + pointObj.z_min) / 2;
	for (int i = 0; i < points.size(); i++) {
		points[i] = points[i] - glm::vec3(x_mid, y_mid, z_mid);
	}

	rev_x = rev_y = rev_z = rev_scale = 0;
	rev_deg = 0;
	if (objFilename == "bunny.obj") {
		rotation = 'x';
	}
	else if(objFilename == "dragon.obj") {
		rotation = 'x';
	}
	else {
		rotation = 'x';
	}

	// Set the color. 
	color = glm::vec3(0, 1, 0);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);

	// Bind to the VAO.
	// This tells OpenGL which data it should be paying attention to
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	/*
	 * TODO: Section 2 and 3. 	 
	 * Following the above example but for vertex normals, 
	 * 1) Generate a new vertex bufferbuffer,
	 * 2) Bind it as a GL_ARRAY_BUFFER type, 
	 * 3) Pass in the data 
	 * 4) Enable the next attribute array (which is 1)
	 * 5) Tell it how to process each vertex using glVertexAttribPointer
	 */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
		normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), 0);


	// HW2: gen EBO for triangles
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

PointCloud::~PointCloud()
{
	// Delete the VBO and the VAO.
	// Failure to delete your VAOs, VBOs and other data given to OpenGL
	// is dangerous and may slow your program and cause memory leaks
	glDeleteBuffers(1, &vbo[0]);
	glDeleteBuffers(1, &vbo[1]);
	glDeleteVertexArrays(1, &vao);
}

PointsObj PointCloud::getPoints(std::string filename) {
	float x, y, z, vnx, vny, vnz;
	float r, g, b;
	char buffer[256];
	int c1, c2, read_c=1;
	int f1, f2, f3;

	FILE* fp;
	PointsObj retObj;
	retObj.x_max = retObj.y_max = retObj.z_max = FLT_MIN;
	retObj.x_min = retObj.y_min = retObj.z_min = FLT_MAX;

	fp = fopen(filename.c_str(), "r");
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }

	c1 = fgetc(fp);
	while ( c1 != EOF) {
		c2 = fgetc(fp);
		if (c1 == 'v' && c2==' ') {
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			if (x > retObj.x_max) { retObj.x_max = x; };
			if (x < retObj.x_min) { retObj.x_min = x; };
			if (y > retObj.y_max) { retObj.y_max = y; };
			if (y < retObj.y_min) { retObj.y_min = y; };
			if (z > retObj.z_max) { retObj.z_max = z; };
			if (z < retObj.z_min) { retObj.z_min = z; };
			retObj.v.push_back(glm::vec3(x, y, z));
		}
		else if (c1 == 'v' && c2 == 'n') {
			fscanf(fp, "%f %f %f", &vnx, &vny, &vnz);
			retObj.vn.push_back(glm::vec3(vnx, vny, vnz));
		}
		else if (c1 == 'f' && c2 == ' ') {
			fscanf(fp, "%d//%d %d//%d %d//%d", &f1, &f1, &f2, &f2, &f3, &f3);
			retObj.f.push_back(glm::ivec3(f1 -1, f2-1, f3-1));
		}

		c1 = c2;
	}
return retObj;
}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	glPointSize(pointSize);
	
	// Draw points 
	//glDrawArrays(GL_TRIANGLES, 0, points.size());

	//draw triangles
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
	//spin(0.05f);
	/*
	 * TODO: Section 3: Modify this function to spin the dragon and bunny about
	 * different axes. Look at the spin function for an idea
	 */ 
}

void PointCloud::updatePointSize(GLfloat size)
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
	if(pointSize + size > 0){
		pointSize += size;
	}
	
}
void PointCloud::move(double x, double y, double z) {
	model = glm::translate(glm::mat4(1.0f), glm::vec3((float) x, (float) y, (float) z))* model;
	rev_x -= x;
	rev_y -= y;
	rev_z -= z;
}
void PointCloud::moveLeft() {
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0)) * model;
	rev_x += 1;
}
void PointCloud::moveRight() {
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0, 0)) * model;
	rev_x -= 1;
}
void PointCloud::moveUp() {
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0)) * model;
	rev_y -= 1;
}
void PointCloud::moveDown() {
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1, 0)) * model;
	rev_y += 1;
}
void PointCloud::moveIn() {
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 1)) * model;
	rev_z -= 1;
}
void PointCloud::moveOut() {
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -1)) * model;
	rev_z += 1;
}
void PointCloud::scaleUp() {
	model = glm::scale(model, glm::vec3(2, 2, 2));
	rev_scale -= 1;
}
void PointCloud::scaleDown() {
	model = glm::scale(model, glm::vec3( 0.5, 0.5, 0.5));
	rev_scale += 1;
}
void PointCloud::resetPosition() {
	model = glm::translate(glm::mat4(1.0f), glm::vec3(rev_x, rev_y, rev_z)) * model;
	rev_x = rev_y = rev_z = 0;
}

void PointCloud::resetOriScale() {
	if (rotation == 'x') {
		model = glm::rotate(model, glm::radians(-rev_deg), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		model = glm::rotate(model, glm::radians(-rev_deg), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	rev_deg = 0;
	float scale_factor = pow(2, rev_scale);
	model = glm::scale(model, glm::vec3(scale_factor, scale_factor, scale_factor));
	rev_scale = 0;
}


void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	if (rotation == 'x') {
		model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
	
	}
	else if (rotation == 'y') {
		model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else {
		model = glm::rotate(model, glm::radians(deg), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	rev_deg += deg;
	if (rev_deg >= 360) rev_deg -= 360;
}

void PointCloud::rotate(float deg, glm::vec3 rot_axis) {
	model = glm::rotate(glm::mat4(1.0f), glm::radians(deg), rot_axis) * model;
}