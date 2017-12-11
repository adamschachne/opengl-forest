#ifndef _NODE_H_
#define _NODE_H_

#include <glm/mat4x4.hpp>

class Node
{
public:
	virtual void draw(glm::mat4 C) = 0;
	virtual void update() = 0;
	virtual ~Node() {}
};

#endif