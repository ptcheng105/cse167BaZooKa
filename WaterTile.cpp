#include "WaterTile.h"
#include <ctime>
WaterTile::WaterTile(GLuint waterProgram, GLfloat toX, GLfloat toZ, GLuint skyboxtextureID, glm::vec3* campos_ptr) {

	waterProg = waterProgram;
	skyBoxTextureID = skyboxtextureID;
	camPos_ptr = campos_ptr;

	model = glm::mat4(1);

	float deltaX = toX / (toX *10);
	float deltaZ = toZ / (toZ *10);
	float nx, nz;
	//set vertices
	for (float x = -toX; x < toX; x = x + deltaX) {
		for (float z = 0; z < toZ; z = z + deltaX) {
			nx = x + deltaX;
			nz = z + deltaZ;
			vertices.push_back(glm::vec3(x, 0, z));
			vertices.push_back(glm::vec3(x, 0, nz));
			vertices.push_back(glm::vec3(nx, 0, nz));
			vertices.push_back(glm::vec3(x, 0, z));
			vertices.push_back(glm::vec3(nx, 0, nz));
			vertices.push_back(glm::vec3(nx, 0, z));
		}
	}
	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

WaterTile::~WaterTile(){
	// Delete the VBOs and the VAO.
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
}

void WaterTile::draw(glm::mat4 projection, glm::mat4 view)
{
	GLfloat time = glfwGetTime();
	glUseProgram(waterProg);
	glUniformMatrix4fv(glGetUniformLocation(waterProg, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(waterProg, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(waterProg, "model"), 1, GL_FALSE, glm::value_ptr(model));//shift it to z side
	glUniform1f(glGetUniformLocation(waterProg, "cur_time"), time);
	glUniform1ui(glGetUniformLocation(waterProg, "gerstner_waves_length"), 3);
	
	glUniform2f(glGetUniformLocation(waterProg, "gerstner_waves[0].direction"), 0.0f, 1.0f);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[0].amplitude"), 2.0);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[0].steepness"), 0.3);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[0].frequency"), 0.3);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[0].speed"), 1.5);
	
	
	glUniform2f(glGetUniformLocation(waterProg, "gerstner_waves[1].direction"), 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[1].amplitude"), 0.6);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[1].steepness"), 0.3);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[1].frequency"), 0.6);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[1].speed"), 5.0);
	
	glUniform2f(glGetUniformLocation(waterProg, "gerstner_waves[2].direction"), -0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[2].amplitude"), 0.3);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[2].steepness"), 0.3);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[2].frequency"), 0.3);
	glUniform1f(glGetUniformLocation(waterProg, "gerstner_waves[2].speed"), 1.5);

	glUniform3fv(glGetUniformLocation(waterProg, "cameraPos"), 1, glm::value_ptr(*camPos_ptr));
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureID);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Unbind from the VAO.
	glBindVertexArray(0);
}