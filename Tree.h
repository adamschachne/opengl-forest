#ifndef TREE_H
#define TREE_H

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

class Tree
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
public:
	float scaleVal;
	float initialScaleVal;
	glm::mat4 toWorld;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> normals;
	std::vector<unsigned int> indices;
	void print();
	Tree(const char* filepath);
	~Tree();
	void parse(const char* filepath);
	void draw(GLuint);
	void update();
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
	const std::vector<std::string> faces = { "../treetexture.jpg" };
	const char * faces2 = "../treetexture.png";
	unsigned int loadTexture(const char *path);
	unsigned int loadMap(std::vector<std::string>);
	unsigned int cubemapTexture;
	GLint treeshader;
};

#endif