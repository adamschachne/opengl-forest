#ifndef _CURVE_H_
#define _CURVE_H_

#include "Geometry.h"
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <errno.h> 
#include <stdexcept>      // std::out_of_range

class Curve : public Node
{	
public:
	Curve(GLuint * sphereVAO, int sphereSize, std::vector<glm::vec3> points, GLuint shaderprogram, unsigned int id);
	~Curve();
	void createCurve();
	void updateCurve();
	void createControlLines();
	void draw(glm::mat4 C);
	void update();
	std::vector<glm::vec3> controlPoints;	
	void setNextCurve(Curve * next);
	void setPrevCurve(Curve * prev);
	std::vector<glm::vec3> points;
	Curve * nextCurve;
	Curve * prevCurve;
	glm::vec3 getPosition(float time);
	float arclength;
	unsigned int id;
private:			
	GLuint * sphereVAO;
	int sphereSize;
	std::vector<Geometry*> controlSpheres;		
	glm::mat4 G, B, Bez;
	glm::mat4 toWorld;
	glm::mat4 initialWorld;
	GLuint controlLineVAO;
	GLuint controlLineVBO;
	GLuint VBO, VAO, EBO;
	GLuint uProjection, uModelview;
	GLuint shaderprogram;
};

#endif