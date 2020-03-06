#include "BezierCurveGeometry.h"
BezierCurveGeometry::BezierCurveGeometry(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float num_of_points) {
	//set model
	this-> model = glm::mat4(1);

	//set Gmat
	Gmat[0] = glm::vec4(p0, 1);
	Gmat[1] = glm::vec4(p1, 1);
	Gmat[2] = glm::vec4(p2, 1);
	Gmat[3] = glm::vec4(p3, 1);
	//set Bmat
	Bmat[0] = glm::vec4(-1, 3, -3, 1);
	Bmat[1] = glm::vec4(3, -6, 3, 0);
	Bmat[2] = glm::vec4(-3, 3, 0, 0);
	Bmat[3] = glm::vec4(1, 0, 0, 0);
 
	//get all the points of the curve
	float delta_t = 1.0 / num_of_points;
	for (float t = 0; t < 1; t += delta_t) {
		curve_points.push_back(getpoint(t));
	}

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	// This tells OpenGL which data it should be paying attention to
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * curve_points.size(),
		curve_points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void BezierCurveGeometry::draw(GLuint shaderProgram, glm::mat4 C)
{
	model = C;
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	glLineWidth(3.0f);
	// Draw points 
	glDrawArrays(GL_LINE_STRIP, 0, curve_points.size());

	//draw triangles
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

glm::vec3 BezierCurveGeometry::getpoint(float t) {
	glm::vec4 Tvec = glm::vec4(pow(t, 3),pow(t,2),t,1);
	glm::vec3 ret = (Gmat * Bmat * Tvec);
	return ret ;
}

BezierCurveGeometry::~BezierCurveGeometry()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void BezierCurveGeometry::update(glm::mat4 C) {

}