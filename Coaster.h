#ifndef _COASTER_H_
#define _COASTER_H_

#include "Curve.h"

class Coaster : public Geometry
{
private:
	const float BUFFER = 10.0f;
	GLuint uModel, uView, uCam;
	float acceleration;	
	float maxHeight;
	float velocity;
	bool paused;
	float boost;
public:
	float ticks;
	Curve * currentCurve;
	void moveHighest();
	Coaster(glm::mat4 world, GLuint * VAO, int size, float maxHeight, float initialHeight, Curve * currentCurve);
	void draw(glm::mat4 C);
	void update();
	void translateTo(glm::vec3 nextPos);
	void pause();
};

#endif