#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <errno.h> 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Node.h"

class Geometry : public Node
{
public:
	Geometry(glm::mat4 world, GLuint * VAO, int size, GLuint shaderprogram, glm::vec3 color, unsigned int id);
	~Geometry();
	void draw();
	void draw(glm::mat4);
	void draw(GLuint shaderpog);
	void translate(glm::vec3);
	void update();
	float increment;
	float maxAngle;
	float currentAngle;
	char type;
	unsigned int id;
	glm::vec3 color;
	glm::mat4 toWorld;
	glm::mat4 initialWorld;
	GLuint VBO, VBO2, VAO, EBO;
	GLuint uProjection, uModelview;
	GLuint shaderprogram;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	int size;
	float initialScaleVal;
	glm::vec3 mid;
};

#endif