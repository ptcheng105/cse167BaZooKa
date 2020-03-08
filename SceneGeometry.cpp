#include "SceneGeometry.h"

PointsObj SceneGeometry::getPoints(std::string filename) {
	//for robot obj file
	float x, y, z, vnx, vny, vnz;
	float r, g, b;
	char buffer[256];
	int c1, c2, read_c = 1;
	int vf1, vf2, vf3;
	int vtf1, vtf2, vtf3;
	int vnf1, vnf2, vnf3;
	std::vector<glm::vec3> vertex_input, normal_input;
	std::vector<int> vertex_indices, normal_indices;

	FILE* fp;
	PointsObj retObj;
	retObj.x_max = retObj.y_max = retObj.z_max = FLT_MIN;
	retObj.x_min = retObj.y_min = retObj.z_min = FLT_MAX;

	fp = fopen(filename.c_str(), "r");
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }

	c1 = fgetc(fp);
	while (c1 != EOF) {
		c2 = fgetc(fp);
		if (c1 == 'v' && c2 == ' ') {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			if (x > retObj.x_max) { retObj.x_max = x; };
			if (x < retObj.x_min) { retObj.x_min = x; };
			if (y > retObj.y_max) { retObj.y_max = y; };
			if (y < retObj.y_min) { retObj.y_min = y; };
			if (z > retObj.z_max) { retObj.z_max = z; };
			if (z < retObj.z_min) { retObj.z_min = z; };
			vertex_input.push_back(glm::vec3(x, y, z));
		}
		else if (c1 == 'v' && c2 == 'n') {
			fscanf(fp, "%f %f %f", &vnx, &vny, &vnz);
			normal_input.push_back(glm::vec3(vnx, vny, vnz));
		}
		else if (c1 == 'f' && c2 == ' ') {
			fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &vf1, &vtf1, &vnf1, &vf2, &vtf2, &vnf2, &vf3, &vtf3, &vnf3);
			//save these indices
			vertex_indices.push_back(vf1);
			vertex_indices.push_back(vf2);
			vertex_indices.push_back(vf3);
			normal_indices.push_back(vnf1);
			normal_indices.push_back(vnf2);
			normal_indices.push_back(vnf3);
		}

		c1 = c2;
	}
	//now reorder the buffer with indices
	for (unsigned int i = 0; i < vertex_indices.size(); i++) {
		retObj.v.push_back(vertex_input[vertex_indices[i]-1]);
		retObj.vn.push_back(normal_input[normal_indices[i]-1]);
	}
	for (unsigned int i = 0; i < vertex_indices.size(); i++) {
		unsigned int f1 = i++;
		unsigned int f2 = i++;
		unsigned int f3 = i;
		retObj.f.push_back(glm::ivec3((int)f1, (int)f2, (int)f3));
	}
	return retObj;
}
PointsObj SceneGeometry::getPoints2(std::string filename) {
	//for normal file
	float x, y, z, vnx, vny, vnz;
	float r, g, b;
	char buffer[256];
	int c1, c2, read_c = 1;
	int f1, f2, f3;

	FILE* fp;
	PointsObj retObj;
	retObj.x_max = retObj.y_max = retObj.z_max = FLT_MIN;
	retObj.x_min = retObj.y_min = retObj.z_min = FLT_MAX;

	fp = fopen(filename.c_str(), "r");
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }

	c1 = fgetc(fp);
	while (c1 != EOF) {
		c2 = fgetc(fp);
		if (c1 == 'v' && c2 == ' ') {
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
			retObj.f.push_back(glm::ivec3(f1 - 1, f2 - 1, f3 - 1));
		}

		c1 = c2;
	}
	return retObj;
};


SceneGeometry::SceneGeometry(std::string objFilename, int use_getPoint_number)
{
	//parse from obj file

	PointsObj pointObj;

	if (use_getPoint_number == 1) {
		pointObj = this->getPoints(objFilename);
	}
	else {
		pointObj = this->getPoints2(objFilename);
	}
	
	points = pointObj.v;
	normals = pointObj.vn;
	indices = pointObj.f;


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


SceneGeometry::~SceneGeometry()
{
	glDeleteBuffers(1, &vbo[0]);
	glDeleteBuffers(1, &vbo[1]);
	glDeleteVertexArrays(1, &vao);
}

void SceneGeometry::draw(GLuint shaderProgram, glm::mat4 C)
{
	model = C;
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	// Bind to the VAO.
	glBindVertexArray(vao);
	//draw triangles
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void SceneGeometry::update(glm::mat4 C) {

}