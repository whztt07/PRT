#ifndef _GLWINDOW_H_
#define _GLWINDOW_H_
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
	void compileShader(const char* vertFileName, const char* fragFileName);
	void renderModel(CAssimpModel* model);
	//void renderPRT(CAssimpModel* model, Color* light, Color** coeffs, int bands);
	void setUniform();
	void clean();
	void changeMatrics();


	ShaderProgram prog;

private:
	int width;
	int height;
	mat4 modelMatrix;
	mat4 viewMatrix;

	GLuint vertexArrayID;
	GLuint vertexbuffer;
	GLuint normalbuffer;

	GLFWwindow* window;

	// GLFW callback
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	// display control
	static bool bAnim;
	static bool locked;
	static GLfloat alpha;
	static GLfloat beta;
	static GLfloat zoom;
	static int cursorX;
	static int cursorY;
};

#endif