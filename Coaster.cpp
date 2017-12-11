#include "Coaster.h"
#include "Window.h"

Coaster::Coaster(glm::mat4 world, GLuint * VAO, int size, float maxHeight, float initialHeight, Curve * currentCurve) : Geometry(world,VAO, size, 0, glm::vec3(0), 0)
{
	this->acceleration = 350.0f;	
	this->maxHeight = maxHeight + BUFFER;
	this->velocity = sqrt(-2.0f * acceleration * (initialHeight - maxHeight));
	this->ticks = 0.0f;
	this->currentCurve = currentCurve;
	this->paused = false;
	this->boost = 0.0f;

	glGenVertexArrays(1, VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(*VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	shaderprogram = LoadShaders("../coaster.vert", "../coaster.frag");
	//shaderprogram = LoadShaders("../shader.vert", "../shader.frag");
	glUseProgram(shaderprogram);
	glUniform1i(glGetUniformLocation(shaderprogram, "skybox"), 0);
}

void Coaster::draw(glm::mat4 C)
{
	glm::mat4 modelview = Window::V * toWorld;

	glUseProgram(shaderprogram);
	uProjection = glGetUniformLocation(shaderprogram, "projection");
	uModel = glGetUniformLocation(shaderprogram, "model");
	uView = glGetUniformLocation(shaderprogram, "view");
	uCam = glGetUniformLocation(shaderprogram, "cameraPos");
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniform3fv(uCam, 1, &Window::Position[0]);

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	//glUniform3f(glGetUniformLocation(shaderprogram, "colorIn"), color.r, color.g, color.b + id / 255.0f);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void Coaster::update()
{
	if (paused) {
		return;
	}

	float height = toWorld[3][1]; // get y offset

	velocity = sqrt(-2.0f * acceleration * (height - maxHeight)) + boost;

	
	//float distance = glm::distance(currentCurve->getPosition(t), glm::vec3(toWorld[3][0], toWorld[3][1], toWorld[3][2]));
	//float curveDist = currentCurve->arclength;
	//std::cout << "velocity/1000 = " << velocity/100.0f << std::endl;
	//std::cout << "distance = " << distance << std::endl;
	//std::cout << "num ticks = " << (velocity/100.0f / distance) << std::endl;
	
	int num_ticks = 1;
	float added_distance = 0.0f;
	glm::vec3 nextPoint = currentCurve->getPosition(ticks + 1.0f / 5000.0f);
	glm::vec3 lastPoint = glm::vec3(toWorld[3][0], toWorld[3][1], toWorld[3][2]);
	while (added_distance < velocity/200) {
		added_distance += glm::distance(nextPoint, lastPoint);
		num_ticks++;
		lastPoint = nextPoint;
		nextPoint = currentCurve->getPosition(ticks + num_ticks*1.0f / 5000.0f);
	}

	ticks += 1/5000.0f * num_ticks;
	
	if (ticks > 1.0f) {
		currentCurve = currentCurve->nextCurve;
		if (currentCurve->id == 16) {
			boost += 1000.0f;
		}
		ticks = ticks - 1.0f;
	}

	if (boost > 0) {
		boost--;
	}
	translateTo(currentCurve->getPosition(ticks));
}

void Coaster::pause() {
	paused = !paused;
}

void Coaster::translateTo(glm::vec3 nextPos) {
	toWorld[3][0] = 0.0f;
	toWorld[3][1] = 0.0f;
	toWorld[3][2] = 0.0f;

	translate(nextPos);
}

void Coaster::moveHighest()
{
	glm::vec3 highest = Window::getHighestPoint();
	this->currentCurve = Window::highestCurve;
	float t = Window::highestTvalue;
	
	this->maxHeight = highest.y + BUFFER;
	this->velocity = sqrt(-2.0f * acceleration * (highest.y - maxHeight));
	this->ticks = t;
}