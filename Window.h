#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include <cstdlib>
#include "OBJObject.h"
#include "Tree.h"
class Window
{
public:
	static int width;
	static int height;
	static float x_old;
	static float y_old;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void print(glm::mat4 matrix);
	static bool colorMode;
	static int targetObject;
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::vec3 trackBallMapping(double x, double y);
	static glm::vec3 Position;
	static bool leftMouseHeld;
	static bool rightMouseHeld;
	static void resetLight();
	static OBJObject * currentLight;
	static glm::vec3 lightcolor;
	static glm::vec3 lightPosition;
	static glm::vec3 lightDirection;
	static int selectedIndex;
	static float lightMode;
	static float lightExponent;
	static float lightField;
	static bool drawBoundingSpheres;
	static void updateCameraVectors();
	static const float YAW;
	static const float PITCH;
	static const float SPEED;
	static const float SENSITIVTY;
	static const float ZOOM;
	static glm::vec3 planesPoints[6];
	static glm::vec3 planeNormals[6];
	static void calculateFrustum();
	static bool cullingEnabled;
	static std::vector<glm::vec3> trackPoints;
	static float highestTvalue;
	static void toggleCameraMode();
	static bool firstPerson;
	static bool forward;
	static bool backward;
	static bool right;
	static bool left;
};
#endif
