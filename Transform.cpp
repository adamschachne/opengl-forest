#include "Transform.h"
#include "Window.h"
 
Transform::Transform(GLuint shaderprogram, glm::mat4 M)
{
	children = new std::list<Node *>();
	this->shaderprogram = shaderprogram;
	this->M = M;	
}

Transform::~Transform()
{
}

void Transform::draw(glm::mat4 C) {
	//  traverse the list of children and call each child node's draw function
	
	// check outside Frustum
	// if outside, return

	if (!isInBounds() && Window::cullingEnabled == true) {
		//std::cout << "x=" << center.x << " y=" << center.y << " z=" << center.z << " is not in bounds" << std::endl;
		return;
	}

	for (auto node = children->begin(); node != children->end(); ++node) {
		(*node)->draw(C);
	}
	
	//when draw(C) is called, multiply matrix M with matrix C.
	//M = C * M;
}

void Transform::update() {
	for (auto node = children->begin(); node != children->end(); ++node) {
		(*node)->update();
	}
}

void Transform::addChild(Node* node)
{
	children->push_back(node);
}

void Transform::removeChild(Node * node)
{
	children->remove(node);
}

bool Transform::isInBounds()
{
	float radius = 60.0f;
	for (int i = 0; i < 6; i++) {	
		float distance = glm::dot((center - Window::planesPoints[i]), Window::planeNormals[i]);
		if (radius >= fabs(distance) && distance >= 0.0)
			continue;
		if (radius <= fabs(distance) && distance >= 0.0)
			continue;		
		if (radius >= fabs(distance) && distance <= 0.0)
			continue;
		return false;
	}
	return true;
}

