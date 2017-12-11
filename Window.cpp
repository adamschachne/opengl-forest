#include "window.h"
#include "Curve.h"

#define NUM_ROBOTS 2500
#define FARDIST 8000.0f
#define NEARDIST 0.1f

const char* window_title = "GLFW Starter Project";
Cube * cube;
OBJObject * sphere;
OBJObject * currentObj;
// selectable objects

Coaster * coaster;

std::vector<Geometry *> Window::selectables;
int Window::selectedIndex = -1;
Geometry * selected = 0;

std::vector<Node *> curves;
OBJObject * Window::currentLight;
int Window::targetObject = 0; // 0 currentObj, 1 directional, 2 point, and 3 spot light
GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

const float Window::YAW = -92.6f;
//const float Window::YAW = 0.0f;
const float Window::PITCH = -27.0f;
//const float Window::PITCH = 0.0f;
const float Window::SPEED = 2.5f;
const float Window::SENSITIVTY = 0.1f;
const float Window::ZOOM = 45.0f;

// Eular Angles
float Yaw = Window::YAW;
float Pitch = Window::PITCH;
// Camera options
float MovementSpeed = 2.5f;
float MouseSensitivity = 0.1f;
float Zoom = 45.0f;
// Camera Attributes
glm::vec3 Window::Position(0.0f, 600.0f, 900.0f);
glm::vec3 OldPosition;
glm::vec3 Front;
glm::vec3 Right;
glm::vec3 Up;
int Window::width;
int Window::height;
glm::mat4 Window::P;
glm::mat4 Window::V;
bool Window::leftMouseHeld = false;
bool Window::rightMouseHeld = false;
float Window::x_old = 0.0f;
float Window::y_old = 0.0f;
bool Window::colorMode = true;
glm::vec3 Window::lightPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Window::lightDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Window::lightcolor = glm::vec3(1.0f, 1.0f, 1.0f);
float Window::lightMode = 0.0f;
float Window::lightExponent = 1.0f;
float Window::lightField = 0.05f;
bool Window::drawBoundingSpheres = false;

bool Window::firstPerson = false;

Curve * curve0;
Curve * curve1;
Curve * curve2;
Curve * curve3;
Curve * curve4;
Curve * curve5;
Curve * curve6;
Curve * curve7;

Curve * Window::highestCurve = curve0;
float Window::highestTvalue = 12.0f;

std::vector<glm::vec3> Window::trackPoints = std::vector<glm::vec3>();

glm::vec3 Window::planesPoints[6] = {};
glm::vec3 Window::planeNormals[6] = {};

double lastTime = glfwGetTime();
int nbFrames = 0;

bool Window::cullingEnabled = true;

void Window::print(glm::mat4 matrix) {
	std::cout << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Window::resetLight() {	
	Window::lightPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	Window::lightDirection = glm::vec3(-20.0f, -20.0f, -20.0f);
	Window::lightcolor = glm::vec3(1.0f, 1.0f, 1.0f);
	Window::lightMode = 0.0f;
	//Window::lightExponent = 2.0f;
	//Window::lightField = 0.3f;
}


void Window::initialize_objects()
{
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	cube = new Cube();
	curves = std::vector<Node *>();
	sphere = new OBJObject("../sphere.obj");
	int sphereSize = sphere->indices.size();

	curve0 = new Curve(&(sphere->VAO), sphereSize, { glm::vec3(-100, -100, -100), glm::vec3(200, 44, 72), glm::vec3(312, 120, 190), glm::vec3(481, 200, 312) }, shaderProgram, 1);
	curve1 = new Curve(&(sphere->VAO), sphereSize, { glm::vec3(481, 200, 312), glm::vec3(650, 280, 434), glm::vec3(700, 280, 500), glm::vec3(400, 280, 500) }, shaderProgram, 4);
	curve2 = new Curve(&(sphere->VAO), sphereSize, { glm::vec3(400, 280, 500), glm::vec3(100, 280, 500), glm::vec3(200,500, 300), glm::vec3(250, 700, 100) }, shaderProgram, 7);
	curve3 = new Curve(&(sphere->VAO), sphereSize, { glm::vec3(250, 700, 100), glm::vec3(300, 900, -100), glm::vec3(600,550,0), glm::vec3(700, 650, 0) }, shaderProgram, 10);
	curve4 = new Curve(&(sphere->VAO), sphereSize, { glm::vec3(700, 650, 0), glm::vec3(800, 750, 0), glm::vec3(30, 40, 500), glm::vec3(-130, -140, 100) }, shaderProgram, 13);
	curve5 = new Curve(&(sphere->VAO), sphereSize, { glm::vec3(-130, -140, 100), glm::vec3(-290, -320, -300), glm::vec3(-400, -100, 300), glm::vec3(-650, 100, 300) }, shaderProgram, 16);
	curve6 = new Curve(&(sphere->VAO), sphereSize, { glm::vec3(-650, 100, 300), glm::vec3(-900, 300, 300), glm::vec3(-60, 320, 500), glm::vec3(-390, 320, 300) }, shaderProgram, 19);
	curve7 = new Curve(&(sphere->VAO), sphereSize, { glm::vec3(-390, 320, 300), glm::vec3(-720, 320, 100), glm::vec3(-400, -244, -272), glm::vec3(-100, -100, -100) }, shaderProgram, 22);

	updateTrackPoints();

	curve0->setNextCurve(curve1); curve0->setPrevCurve(curve7);
	curve1->setNextCurve(curve2); curve1->setPrevCurve(curve0);
	curve2->setNextCurve(curve3); curve2->setPrevCurve(curve1);
	curve3->setNextCurve(curve4); curve3->setPrevCurve(curve2);
	curve4->setNextCurve(curve5); curve4->setPrevCurve(curve3);
	curve5->setNextCurve(curve6); curve5->setPrevCurve(curve4);
	curve6->setNextCurve(curve7); curve6->setPrevCurve(curve5);
	curve7->setNextCurve(curve0); curve7->setPrevCurve(curve6);

	curves.push_back(curve0);
	curves.push_back(curve1);
	curves.push_back(curve2);
	curves.push_back(curve3);
	curves.push_back(curve4);
	curves.push_back(curve5);
	curves.push_back(curve6);
	curves.push_back(curve7);

	glm::mat4 coasterWorld = glm::mat4(1.0f) * glm::scale(glm::mat4(1.0f), glm::vec3(30.0f, 30.0f, 30.0f));
	glm::mat4 start = selectables[0]->toWorld;
	coasterWorld = glm::translate(glm::mat4(1.0f), glm::vec3(start[3][0], start[3][1], start[3][2])) * coasterWorld;
	coaster = new Coaster(coasterWorld, &sphere->VAO, sphereSize, 900, -100, curve0);

	updateCameraVectors();
}

void Window::updateTrackPoints() {
	trackPoints.clear();

	for (int i = 0; i < curve0->points.size(); i++) {
		trackPoints.push_back(curve0->points[i]);
	}
	for (int i = 0; i < curve1->points.size(); i++) {
		trackPoints.push_back(curve1->points[i]);
	}
	for (int i = 0; i < curve2->points.size(); i++) {
		trackPoints.push_back(curve2->points[i]);
	}
	for (int i = 0; i < curve3->points.size(); i++) {
		trackPoints.push_back(curve3->points[i]);
	}
	for (int i = 0; i < curve4->points.size(); i++) {
		trackPoints.push_back(curve4->points[i]);
	}
	for (int i = 0; i < curve5->points.size(); i++) {
		trackPoints.push_back(curve5->points[i]);
	}
	for (int i = 0; i < curve6->points.size(); i++) {
		trackPoints.push_back(curve6->points[i]);
	}
	for (int i = 0; i < curve7->points.size(); i++) {
		trackPoints.push_back(curve7->points[i]);
	}
}

glm::vec3 Window::getHighestPoint()
{
	glm::vec3 highest = curve0->points[0];
	Curve * highest_curve = curve0;
	float t_value = 0.0f;

	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 150.0f) {
		if (curve0->getPosition(t).y > highest.y) {
			t_value = t;
			highest_curve = curve0;
			highest = curve0->getPosition(t);
		}
	}
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 150.0f) {
		if (curve1->getPosition(t).y > highest.y) {
			t_value = t;
			highest_curve = curve1;
			highest = curve1->getPosition(t);
		}
	}
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 150.0f) {
		if (curve2->getPosition(t).y > highest.y) {
			t_value = t;
			highest_curve = curve2;
			highest = curve2->getPosition(t);
		}
	}
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 150.0f) {
		if (curve3->getPosition(t).y > highest.y) {
			t_value = t;
			highest_curve = curve3;
			highest = curve3->getPosition(t);
		}
	}
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 150.0f) {
		if (curve4->getPosition(t).y > highest.y) {
			t_value = t;
			highest_curve = curve4;
			highest = curve4->getPosition(t);
		}
	}
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 150.0f) {
		if (curve5->getPosition(t).y > highest.y) {
			t_value = t;
			highest_curve = curve5;
			highest = curve5->getPosition(t);
		}
	}
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 150.0f) {
		if (curve6->getPosition(t).y > highest.y) {
			t_value = t;
			highest_curve = curve6;
			highest = curve6->getPosition(t);
		}
	}
	for (float t = 0.0f; t <= 1.0f; t += 1.0f / 150.0f) {
		if (curve7->getPosition(t).y > highest.y) {
			t_value = t;
			highest_curve = curve7;
			highest = curve7->getPosition(t);
		}
	}

	Window::highestCurve = highest_curve;
	Window::highestTvalue = t_value;
	return highest;
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	delete(sphere);
	//delete(robots);
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		//P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
		P = glm::perspective(glm::radians(Zoom), (float)width / (float)height, NEARDIST, FARDIST);
		//V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		V = glm::lookAt(Position, Position + Front, Up);
	}
}

void Window::idle_callback()
{
	coaster->update();
}

void Window::display_callback(GLFWwindow* window)
{
	
	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
										 // printf and reset timer
		//printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}

	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	for (int i = 0; i < curves.size(); i++) {
		curves[i]->draw(glm::mat4(1.0f));
	}
	
	coaster->draw(glm::mat4(1.0f));

	if (firstPerson) {
		Position = glm::vec3(coaster->toWorld[3][0], coaster->toWorld[3][1], coaster->toWorld[3][2]);
		V = glm::lookAt(Position, coaster->currentCurve->getPosition(coaster->ticks + 1/150.0f), Up);
	}

	cube->draw();
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			// rotates
			leftMouseHeld = true;
		}	
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			rightMouseHeld = true;
			unsigned char pix[4];
			glReadPixels(x_old, height - y_old, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pix);
			std::cout << (unsigned int)pix[2] << std::endl;;
			if ((unsigned int)pix[2] > 24) {
				selected = 0;
				selectedIndex = -1;
				return;
			}
			else {
				try {
					selected = selectables.at((unsigned int)pix[2] - 1);
					//selected = selectables[(unsigned int)pix[2] - 1];
					selectedIndex = (unsigned int)pix[2] - 1;
				}
				catch (const std::exception& ex) {
					std::cerr << ex.what() << std::endl;
				}
			}			
		}
	}
	if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			leftMouseHeld = false;
		}	
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			rightMouseHeld = false;
		}
	}
}

glm::vec3 Window::trackBallMapping(double x, double y) {
	glm::vec3 v;
	float d;
	v.x = (2.0*x - Window::width) / Window::width;
	v.y = (Window::height - 2.0*y) / Window::height;
	v.z = 0.0;
	d = glm::length(v);
	d = (d<1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d*d);
	v = glm::normalize(v);
	return v;
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (leftMouseHeld) {
		float xoffset = xpos - x_old;
		float yoffset = ypos - y_old;

		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch -= yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}
	
	if (rightMouseHeld) {
		if (selected != 0) {
			glm::vec3 upamt = (float)(xpos - x_old) * Right;
			glm::vec3 rightamt = (float)(ypos - y_old) * Up;
			selected->translate(upamt);
			selected->translate(-rightamt);
		}
	}
	
	x_old = xpos;
	y_old = ypos;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Window::updateCameraVectors()
{
	//std::cout << "yaw = " << Yaw << "  " << "Pitch = " << Pitch << std::endl;
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, cam_up));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));

	V = glm::lookAt(Position, Position + Front, Up);
	//print(V);
	calculateFrustum();
}

void Window::calculateFrustum()
{
	float ratio = (float)width / (float)height;
	float far = FARDIST;
	float nearHeight = (NEARDIST * tan(glm::radians(Zoom) / 2.0)) * 2.0;
	float nearWidth = nearHeight * ratio;

	glm::vec3 e = Position;
	glm::vec3 d = Position + Front;
	glm::vec3 z = glm::normalize(e - d);
	glm::vec3 x = glm::normalize(glm::cross(Up, z));
	glm::vec3 y = glm::cross(z, x);
	glm::vec3 yh = (nearHeight / 2.0f) * y;
	glm::vec3 xw = (nearWidth / 2.0f) * x;
	glm::vec3 zd = z * NEARDIST;
	glm::vec3 nearPlaneCenter = e - zd;
	glm::vec3 normal = -z;

	planesPoints[0] = nearPlaneCenter;
	planeNormals[0] = normal;

	zd = z * far;
	glm::vec3 farCenter = e - zd;

	planesPoints[1] = farCenter;
	planeNormals[1] = -normal;

	planesPoints[2] = nearPlaneCenter + yh;
	planeNormals[2] = glm::cross(glm::normalize(nearPlaneCenter + yh - e), x);

	planesPoints[3] = nearPlaneCenter - yh;
	planeNormals[3] = glm::cross(x, glm::normalize(nearPlaneCenter - yh - e));

	planesPoints[4] = nearPlaneCenter - xw;
	planeNormals[4] = glm::cross(glm::normalize(nearPlaneCenter - xw - e), y);

	planesPoints[5] = nearPlaneCenter + xw;
	planeNormals[5] = glm::cross(y, glm::normalize(nearPlaneCenter + xw - e));
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	if (Zoom >= 1.0f && Zoom <= 90.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 90.0f)
		Zoom = 90.0f;

	P = glm::perspective(glm::radians(Zoom), (float)width / (float)height, NEARDIST, FARDIST);

	calculateFrustum();

}

void Window::toggleCameraMode() {
	firstPerson = !firstPerson;	
	if (firstPerson) {
		OldPosition = Position;
	}
	else {
		Position = OldPosition;
		Zoom = 45.0f;
		Yaw = -92.6f;
		Pitch = -27.0f;
		updateCameraVectors();
	}
	
	std::cout << "firstPerson mode " << (firstPerson ? "enabled" : "disabled") << std::endl;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		switch (key) {
			case GLFW_KEY_ESCAPE:
				// Close the window. This causes the program to also terminate.
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_N:
				colorMode = !colorMode;
				break;
			case GLFW_KEY_P:
				coaster->pause();
				break;
			case GLFW_KEY_H:
				coaster->moveHighest();
				break;
			case GLFW_KEY_0:
				Window::targetObject = 0;
				break;
			case GLFW_KEY_1:
				if (currentLight != 0) {
					resetLight();
				}
				currentLight = 0;
				Window::targetObject = 1;
				//std::cout << "directional light selected" << std::endl;			
				Window::lightMode = 0;
				Window::lightPosition = glm::vec3(2.0f, 2.0f, 5.0f);
				Window::lightDirection = glm::vec3(-2.0f, -2.0f, -2.0f);
				break;
			case GLFW_KEY_C:
				Window::toggleCameraMode();
				break;
			/*case GLFW_KEY_C:
				Window::cullingEnabled = !Window::cullingEnabled;
				std::cout << "culling: " << Window::cullingEnabled << std::endl;
				break;*/
		}
	}
}