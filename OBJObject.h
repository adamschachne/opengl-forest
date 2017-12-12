#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <string>
#include <errno.h> 
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	//void draw(GLuint shaderProgram);
	float shininess;
};

class OBJObject
{
private:
	//std::vector<std::pair<char, glm::vec3>> vertices;
	glm::mat4 rotation;
	glm::mat4 orbitMat;
	glm::mat4 translateMat;
	glm::mat4 spinMat;
	std::vector<std::string> split(std::string&, char);
	float orbitDeg;
	float angle;
	glm::vec3 mid;
	Material mat;
public:
	float scaleVal;
	float initialScaleVal;
	glm::mat4 toWorld;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoordinates;

	std::vector<glm::vec3> mappedVertices;
	std::vector<glm::vec3> mappedNormals;
	std::vector<glm::vec2> mappedTextureCoordinates;

	std::vector<unsigned int> indices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> textureIndices;
	void print();
	OBJObject(const char* filepath);
	~OBJObject();
	void parse(const char* filepath);
	void draw(GLuint);
	void update();
	void spin(float);
	void orbit(float);
	void scale(float);
	void translate(float, float, float);
	void reset();
	glm::vec3 offset;
	float orbitAmt;
	// These variables are needed for the shader program
	GLuint VBO, VBO2, VAO, EBO;
	GLuint uProjection, uModelview;
	float y_min;
	float y_max;
};

#endif