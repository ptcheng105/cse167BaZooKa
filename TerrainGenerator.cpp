#include "TerrainGenerator.h"
#include <stdlib.h>  
#include <time.h>  
TerrainGenerator::TerrainGenerator(int divisions, float size, float height, std::string filename) {
	verticesCount = (divisions + 1) * (divisions + 1);		// VerticesCount = (maxDivisions + 1) * (maxDivisions + 1)	If there are 4 faces, then there are 5 vertices
	vertices = std::vector<glm::vec3>(verticesCount);		// set size of vertices vector to verticesCount
	uvs.resize(verticesCount);								// should end up with same amount of uvs as vertices
	triIndeces.resize(divisions * divisions * 2);			// will be size maxDivisions * maxDivisions * 2 triangles per face * 3 vertices per triangle

	float halfSize = size * 0.5f;
	float divisionSize = size / divisions;
	int triOffset = 0;

	// generate grid of vertices as well as uvs and triangle indeces
	for (int i = 0; i <= divisions; i++) {
		for (int j = 0; j <= divisions; j++) {
			vertices[i * (divisions + 1) + j] = glm::vec3(-halfSize + j * divisionSize, 0.0f, halfSize - i * divisionSize);
			uvs[i * (divisions + 1) + j] = glm::vec2(((float)i) / divisions, ((float)j) / divisions);
			if (i < divisions && j < divisions) {
				int topLeft = i * (divisions + 1) + j;										// get triangle indeces of triangles
				int botLeft = (i + 1) * (divisions + 1) + j;

				triIndeces[triOffset] = glm::ivec3(topLeft, topLeft + 1, botLeft + 1);		// set triangles in indeces vector
				triIndeces[triOffset + 1] = glm::ivec3(topLeft, botLeft + 1, botLeft);

				triOffset += 2;
			}
		}
	}

	//initialize random seed generator based on time
	srand(time(NULL));

	// begin diamond square algorithm
	// initialize corners of terrain grid
	vertices[0].y = -height + rand() % (2 * (int)height + 1);
	vertices[divisions].y = -height + rand() % (2 * (int)height + 1);
	vertices[verticesCount - 1].y = -height + rand() % (2 * (int)height + 1);
	vertices[verticesCount - 1 - divisions].y = -height + rand() % (2 * (int)height + 1);


	// begin algorithm iterations
	int iterations = (int)log2(divisions);
	int numSquares = 1;
	int squareSize = divisions;
	
	for (int i = 0; i < iterations; i++) {
		int row = 0;
		for (int j = 0; j < numSquares; j++) {
			int col = 0;
			for (int k = 0; k < numSquares; k++) {
				DiamondSquare(row, col, squareSize, height, divisions);
				col += squareSize;
			}
			row += squareSize;
		}
		numSquares *= 2;
		squareSize /= 2;
		height *= 0.7f;	//adjust to liking
	}

	// now that all vertices are set, find all surface normals

	for (int i = 0; i < triIndeces.size(); i++) {
		glm::vec3 p1 = vertices[triIndeces[i].x];
		glm::vec3 p2 = vertices[triIndeces[i].y];
		glm::vec3 p3 = vertices[triIndeces[i].z];
		
		glm::vec3 A = p2 - p1;
		glm::vec3 B = p3 - p1;

		glm::vec3 normal = glm::cross(A, B);
		normals.push_back(normal);
	}
	//create cliff
	for (int i = 0; i < vertices.size(); i++) {
		//if z > 0, y = -10
		GLfloat z_val= vertices[i].z;
		glm::vec3 offset = glm::vec3(0,0,0);
		if (z_val > 20) {
			offset = glm::vec3(0, -30, 0);
		}
		else if (z_val > -80) {
			float y_val = pow((-0.04 * z_val - 1), 3);
			offset = glm::vec3(0, y_val, 0);
		}
		else {
			float y_val = 20 * log(-z_val - 80) + 20;
			offset = glm::vec3(0, y_val, 0);
		}

		vertices[i] = vertices[i] + offset;
	}

	model = glm::mat4(1.0f);	// Terrain can have a model I guess?
	setBuffers();
	genTexture(filename);
}

void TerrainGenerator::DiamondSquare(int row, int col, int size, float offset, int divisions) {
	int halfSize = (int)(size * 0.5f);
	int topLeft = row * (divisions + 1) + col;
	int botLeft = (row + size) * (divisions + 1) + col;

	int mid = (int)(row + halfSize) * (divisions + 1) + (int)(col + halfSize);
	vertices[mid].y = (vertices[topLeft].y + vertices[topLeft + size].y + vertices[botLeft].y + vertices[botLeft + size].y)*.25f + -offset + rand() % (2 * (int)offset + 1);

	vertices[topLeft + halfSize].y = (vertices[topLeft].y + vertices[topLeft + size].y + vertices[mid].y) / 3 + -offset + rand() % (2 * (int)offset + 1);
	vertices[mid - halfSize].y = (vertices[topLeft].y + vertices[botLeft].y + vertices[mid].y) / 3 + -offset + rand() % (2 * (int)offset + 1);
	vertices[mid + halfSize].y = (vertices[topLeft + size].y + vertices[botLeft + size].y + vertices[mid].y) / 3 - offset + rand() % (2 * (int)offset + 1);
	vertices[botLeft + halfSize].y = (vertices[botLeft].y + vertices[botLeft + size].y + vertices[mid].y) / 3 + -offset + rand() % (2 * (int)offset + 1);
}

void TerrainGenerator::genTexture(std::string filename) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void TerrainGenerator::setBuffers() {
	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);
	// Bind to the VAO.
	// This tells OpenGL which data it should be paying attention to
	glBindVertexArray(vao);
	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
		normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvs.size(),
		uvs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(GLfloat), 0);
	
	
	// gen EBO for triangles
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * triIndeces.size(), triIndeces.data(), GL_STATIC_DRAW);
	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void TerrainGenerator::draw(GLuint program, glm::mat4 projection, glm::mat4 view, glm::mat4 C) {
	model = C;

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindTexture(GL_TEXTURE_2D, texture);
	// Bind to the VAO.
	glBindVertexArray(vao);
	//draw triangles
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 3 * triIndeces.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

TerrainGenerator::~TerrainGenerator() {
	glDeleteBuffers(2, vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}