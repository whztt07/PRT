#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "CAssimpModel.h"
using glm::vec3;
using glm::mat4;

class GLWindow
{
public:
	//GLWindow(void);
	GLWindow(int _width = 480, int _height = 480);
	~GLWindow(void);
	void initGLFW();
	void initGL();
	void compileShader();
	void renderGL();
	void setUniform();
	void clean();
	void changeMatrics();

private:
	int width;
	int height;
	mat4 modelMatrix;
	mat4 viewMatrix;

	ShaderProgram prog;
	GLuint vertexArrayID;
	GLuint vertexbuffer;
	GLuint normalbuffer;

	GLFWwindow* window;
	CAssimpModel model;

	// GLFW callback
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	// display
	static bool bAnim;
	static GLfloat alpha;
	static GLfloat beta;
	static GLfloat zoom;
	static bool locked;
	static int cursorX;
	static int cursorY;
};

