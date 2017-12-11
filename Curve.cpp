#include "Curve.h"
#include "Window.h"

Curve::Curve(GLuint * sphereVAO, int sphereSize, std::vector<glm::vec3> points, GLuint shaderprogram, unsigned int id)
{
	this->sphereVAO = sphereVAO;
	this->sphereSize = sphereSize;
	this->controlPoints = points;
	this->shaderprogram = shaderprogram;
	this->toWorld = glm::mat4(1.0f);
	this->controlSpheres = std::vector<Geometry*>();
	this->nextCurve = 0;
	this->id = id;
	this->arclength = 0.0f;

	// create matrix
	this->G = glm::mat4(
		controlPoints[0].x, controlPoints[1].x, controlPoints[2].x, controlPoints[3].x,
		controlPoints[0].y, controlPoints[1].y, controlPoints[2].y, controlPoints[3].y,
		controlPoints[0].z, controlPoints[1].z, controlPoints[2].z, controlPoints[3].z,
		0.0f, 0.0f, 0.0f, 0.0f
	);

	this->B = glm::mat4(
		-1.0f, 3.0f, -3.0f, 1.0f,
		3.0f, -6.0f, 3.0f, 0.0f,
		-3.0f, 3.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f
	);

	createCurve();
}

Curve::~Curve()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Curve::createCurve() {

	// calculate all points
	this->points = std::vector<glm::vec3>();
	glm::vec3 lastPoint(0.0f);
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 150.0f) {
		glm::vec4 T = glm::vec4(pow(t, 3), pow(t, 2), t, 1.0f);
		glm::vec4 x = (T*B*G);
		if (t > 0.0f) {
			arclength += glm::distance(glm::vec3(x.x, x.y, x.z), lastPoint);
		}
		lastPoint = glm::vec3(x.x, x.y, x.z);
		points.push_back(glm::vec3(x.x, x.y, x.z));
	}

	//add new control spheres 
	glm::mat4 scaled = toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));

	Geometry * sphere1 = new Geometry(glm::translate(glm::mat4(1.0f), controlPoints[0]) * scaled, sphereVAO, sphereSize, shaderprogram, glm::vec3(1.0f, 0.0f, 0.0f), id);
	controlSpheres.push_back(sphere1);
	Window::selectables.push_back(sphere1);


	Geometry * sphere2 = new Geometry(glm::translate(glm::mat4(1.0f), controlPoints[1]) * scaled, sphereVAO, sphereSize, shaderprogram, glm::vec3(0.0f, 1.0f, 0.0f), id + 1);
	controlSpheres.push_back(sphere2);
	Window::selectables.push_back(sphere2);


	Geometry * sphere3 = new Geometry(glm::translate(glm::mat4(1.0f), controlPoints[2]) * scaled, sphereVAO, sphereSize, shaderprogram, glm::vec3(0.0f, 1.0f, 0.0f), id + 2);
	controlSpheres.push_back(sphere3);
	Window::selectables.push_back(sphere3);


	// bind buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);
}

void Curve::updateCurve() 
{	
	// reobtain G matrix
	this->G = glm::mat4(
		controlPoints[0].x, controlPoints[1].x, controlPoints[2].x, controlPoints[3].x,
		controlPoints[0].y, controlPoints[1].y, controlPoints[2].y, controlPoints[3].y,
		controlPoints[0].z, controlPoints[1].z, controlPoints[2].z, controlPoints[3].z,
		0.0f, 0.0f, 0.0f, 0.0f
	);

	// clear points and reobtain new ones
	this->points = std::vector<glm::vec3>();
	glm::vec3 lastPoint(0.0f);
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 150.0f) {
		glm::vec4 T = glm::vec4(pow(t, 3), pow(t, 2), t, 1.0f);
		glm::vec4 x = (T*B*G);
		if (t > 0.0f) {
			arclength += glm::distance(glm::vec3(x.x, x.y, x.z), lastPoint);
		}
		lastPoint = glm::vec3(x.x, x.y, x.z);
		points.push_back(glm::vec3(x.x, x.y, x.z));
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);	

	Window::updateTrackPoints();
	createControlLines();
}

void Curve::createControlLines() 
{
	std::vector<glm::vec3> controlLinePoints;
	controlLinePoints.push_back(controlPoints[2]);
	controlLinePoints.push_back(nextCurve->controlPoints[1]);

	glDeleteVertexArrays(1, &controlLineVAO);
	glDeleteBuffers(1, &controlLineVBO);

	glGenVertexArrays(1, &controlLineVAO);
	glGenBuffers(1, &controlLineVBO);
	glBindVertexArray(controlLineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, controlLineVBO);
	glBufferData(GL_ARRAY_BUFFER, controlLinePoints.size() * sizeof(glm::vec3), controlLinePoints.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);	
}

void Curve::draw(glm::mat4 C)
{
	int i = 0;
	int indexModified = -1;
	bool changed = false;
	glm::vec3 translationAmount;

	for (auto node = controlSpheres.begin(); node != controlSpheres.end(); ++node) {
		glm::mat4 geoWorld = (*node)->toWorld;
		glm::vec3 spherePos = glm::vec3(geoWorld[3][0], geoWorld[3][1], geoWorld[3][2]);
		glm::vec3 controlPointPos = controlPoints[i];
		
		controlPoints[i] = spherePos;
		
		if (controlPointPos != spherePos) {
			changed = true;
			indexModified = i;
			translationAmount = (spherePos - controlPointPos);
			break; // only one sphere can change, no need to check others
		}

		i++;
	}

	if (changed) {
		//std::cout << "id: " << indexModified << " translation amount = " << translationAmount.x << " " << translationAmount.y << " " << translationAmount.z << std::endl;

		if (indexModified == 0) {
			// mirror previous curve's anchor
			prevCurve->controlPoints[3] = controlPoints[0];
			
			controlPoints[1] += translationAmount;
			controlSpheres[1]->toWorld = glm::translate(glm::mat4(1.0f), translationAmount) * controlSpheres[1]->toWorld;

			prevCurve->controlPoints[2] += translationAmount;
			prevCurve->controlSpheres[2]->toWorld = glm::translate(glm::mat4(1.0f), translationAmount) * prevCurve->controlSpheres[2]->toWorld;
			// update
			prevCurve->updateCurve();
			updateCurve();
		}
		else if (indexModified == 1) {
			// modify previous curve's third control point (i=2)
			prevCurve->controlPoints[2] -= translationAmount;
			// translate previous curves sphere
			prevCurve->controlSpheres[2]->toWorld = glm::translate(glm::mat4(1.0f), -translationAmount) * prevCurve->controlSpheres[2]->toWorld;

			// update
			prevCurve->updateCurve();
			updateCurve();
		}
		else if (indexModified == 2) {
			// modify next curve's second point (i=1)
			nextCurve->controlPoints[1] -= translationAmount;
			// translate previous curves sphere
			nextCurve->controlSpheres[1]->toWorld = glm::translate(glm::mat4(1.0f), -translationAmount) * nextCurve->controlSpheres[1]->toWorld;

			// update
			nextCurve->updateCurve();
			updateCurve();
		}		
	}


	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderprogram, "projection");
	uModelview = glGetUniformLocation(shaderprogram, "modelview");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glLineWidth(3.0f);
	// draw curve
	glBindVertexArray(VAO);
	if (id == 16) {
		glUniform3f(glGetUniformLocation(shaderprogram, "colorIn"), 1.0f, 0.0f, 0.0f); // fast track
	}
	else {
		glUniform3f(glGetUniformLocation(shaderprogram, "colorIn"), 1.0f, 1.0f, 1.0f); // white track
	}
	
	glDrawArrays(GL_LINE_STRIP, 0, points.size());
	glBindVertexArray(0);

	// draw control line
	glBindVertexArray(controlLineVAO);
	glUniform3f(glGetUniformLocation(shaderprogram, "colorIn"), 1.0f, 1.0f, 0.0f); // yellow line
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(0);

	for (auto node = controlSpheres.begin(); node != controlSpheres.end(); ++node) {
		(*node)->draw(C);
	}

}

void Curve::update()
{
}

void Curve::setNextCurve(Curve * next)
{
	this->nextCurve = next;
}

void Curve::setPrevCurve(Curve * prev)
{
	this->prevCurve = prev;
	createControlLines();
}

glm::vec3 Curve::getPosition(float time)
{
	glm::vec4 T = glm::vec4(pow(time, 3), pow(time, 2), time, 1.0f);
	glm::vec4 x = (T*B*G);
		
	return glm::vec3(x.x, x.y, x.z);
}
