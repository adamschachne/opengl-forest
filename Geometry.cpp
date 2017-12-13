#include "Geometry.h"
#include "Window.h"

Geometry::Geometry(glm::mat4 world, GLuint * VAO, int size, GLuint shaderprogram, glm::vec3 color, unsigned int id)
{
	this->toWorld = world;
	this->VAO = *VAO;
	this->size = size;
	this->shaderprogram = shaderprogram;
	this->color = color;
	this->id = id;
}

Geometry::~Geometry()
{
	//std::cout << "deleting Geometry object" << std::endl;
}

void Geometry::draw()
{
	glUseProgram(shaderprogram);
	glUniform1i(glGetUniformLocation(shaderprogram, "texture_diffuse1"), 1);
	glActiveTexture(GL_TEXTURE1);

	glBindTexture(GL_TEXTURE_2D, 2);
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;

	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderprogram, "projection");
	uModelview = glGetUniformLocation(shaderprogram, "modelview");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	glDepthFunc(GL_LEQUAL);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Geometry::draw(glm::mat4)
{
}

void Geometry::translate(glm::vec3 offset) {
	toWorld = glm::translate(glm::mat4(1.0f), offset) * toWorld;
}

void Geometry::update()
{	
}