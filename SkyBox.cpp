#include "SkyBox.h"

using namespace std;

GLuint SkyBox::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

SkyBox::SkyBox() {
    vector<std::string> faces =
    {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
		"skybox/front.jpg",
        "skybox/back.jpg"
        
    };
    skyBoxTextureID = loadCubemap(faces);

	float skyboxVertices[] = {
		// positions          
		-500.0f,  500.0f, -500.0f,
		-500.0f, -500.0f, -500.0f,
		500.0f, -500.0f, -500.0f,
		500.0f, -500.0f, -500.0f,
		500.0f,  500.0f, -500.0f,
		-500.0f,  500.0f, -500.0f,

		-500.0f, -500.0f,  500.0f,
		-500.0f, -500.0f, -500.0f,
		-500.0f,  500.0f, -500.0f,
		-500.0f,  500.0f, -500.0f,
		-500.0f,  500.0f,  500.0f,
		-500.0f, -500.0f,  500.0f,

		500.0f, -500.0f, -500.0f,
		500.0f, -500.0f,  500.0f,
		500.0f,  500.0f,  500.0f,
		500.0f,  500.0f,  500.0f,
		500.0f,  500.0f, -500.0f,
		500.0f, -500.0f, -500.0f,

		-500.0f, -500.0f,  500.0f,
		-500.0f,  500.0f,  500.0f,
		500.0f,  500.0f,  500.0f,
		500.0f,  500.0f,  500.0f,
		500.0f, -500.0f,  500.0f,
		-500.0f, -500.0f,  500.0f,

		-500.0f,  500.0f, -500.0f,
		500.0f,  500.0f, -500.0f,
		500.0f,  500.0f,  500.0f,
		500.0f,  500.0f,  500.0f,
		-500.0f,  500.0f,  500.0f,
		-500.0f,  500.0f, -500.0f,

		-500.0f, -500.0f, -500.0f,
		-500.0f, -500.0f,  500.0f,
		500.0f, -500.0f, -500.0f,
		500.0f, -500.0f, -500.0f,
		-500.0f, -500.0f,  500.0f,
		500.0f, -500.0f,  500.0f
	};

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),
		skyboxVertices, GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);

}

SkyBox::~SkyBox() {
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void SkyBox::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void SkyBox::update()
{
	//do nothing
}