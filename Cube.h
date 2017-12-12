#ifndef _CUBE_H_
#define _CUBE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
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
#include "stb_image.h"
#include <vector>

class Cube
{
public:
	Cube();
	~Cube();

	glm::mat4 toWorld;

	GLint skyboxshader;

	void draw();
	void update();
	void spin(float);

	unsigned int loadCubemap(std::vector<std::string>);

	unsigned int cubemapTexture;

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
};

// Define the coordinates and indices needed to draw the cube. Note that it is not necessary
// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
// This just looks nicer since it's easy to tell what coordinates/indices belong where.


const float skyboxVertices[] = {
	// positions          
	-1000.0f,  1000.0f, -1000.0f,
	-1000.0f, -1000.0f, -1000.0f,
	1000.0f, -1000.0f, -1000.0f,
	1000.0f, -1000.0f, -1000.0f,
	1000.0f,  1000.0f, -1000.0f,
	-1000.0f,  1000.0f, -1000.0f,

	-1000.0f, -1000.0f,  1000.0f,
	-1000.0f, -1000.0f, -1000.0f,
	-1000.0f,  1000.0f, -1000.0f,
	-1000.0f,  1000.0f, -1000.0f,
	-1000.0f,  1000.0f,  1000.0f,
	-1000.0f, -1000.0f,  1000.0f,

	1000.0f, -1000.0f, -1000.0f,
	1000.0f, -1000.0f,  1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	1000.0f,  1000.0f, -1000.0f,
	1000.0f, -1000.0f, -1000.0f,

	-1000.0f, -1000.0f,  1000.0f,
	-1000.0f,  1000.0f,  1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	1000.0f, -1000.0f,  1000.0f,
	-1000.0f, -1000.0f,  1000.0f,

	-1000.0f,  1000.0f, -1000.0f,
	1000.0f,  1000.0f, -1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	1000.0f,  1000.0f,  1000.0f,
	-1000.0f,  1000.0f,  1000.0f,
	-1000.0f,  1000.0f, -1000.0f,

	-1000.0f, -1000.0f, -1000.0f,
	-1000.0f, -1000.0f,  1000.0f,
	1000.0f, -1000.0f, -1000.0f,
	1000.0f, -1000.0f, -1000.0f,
	-1000.0f, -1000.0f,  1000.0f,
	1000.0f, -1000.0f,  1000.0f
};

const std::vector<std::string> faces
{
	"../iceflow_rt.jpg",
	"../iceflow_lf.jpg",
	"../iceflow_up.jpg",
	"../iceflow_dn.jpg",
	"../iceflow_bk.jpg",
	"../iceflow_ft.jpg"
};

// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral


#endif

