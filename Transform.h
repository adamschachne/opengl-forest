#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"
#include <list>
#include <vector>
#include "OBJObject.h"
#include "Geometry.h"

class Transform : public Node
{
public:
	Transform(GLuint shaderprogram, glm::mat4 M);
	~Transform();
	void draw(glm::mat4 C);
	void update();
	void addChild(Node*);
	void removeChild(Node*);
	bool isInBounds();
private:
	glm::vec3 center;
	glm::mat4 M;
	std::list<Node*> * children;
	GLuint shaderprogram;
	std::vector<GLuint *> VAOs;
	std::vector<int> sizes;
	std::vector<std::vector<glm::vec3>> verts;
	std::vector<std::vector<glm::vec3>> norms;
	std::vector<std::vector<unsigned int>> inds;
};

#endif