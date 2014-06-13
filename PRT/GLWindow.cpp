#include "GLWindow.h"

bool GLWindow::bAnim = false;
GLfloat GLWindow::alpha = 210.f;
GLfloat GLWindow::beta = -70.f;
GLfloat GLWindow::zoom = 2.f;
bool GLWindow::locked = true;
int GLWindow::cursorX = 0;
int GLWindow::cursorY = 0;

GLWindow::GLWindow(int _width, int _height) 
	: width(_width), height(_height), prog(), model(prog)
{
	initGLFW();

	// Initialize GLEW
	if (GLEW_OK != glewInit()) {
		printf("[ERROR] Failed to initialize GLEW\n");
		exit(EXIT_FAILURE);
	}

	model.LoadModelFromFile("models/bunny.obj");
	initGL();
	compileShader();

	setUniform();
};

GLWindow::~GLWindow(void)
{
}

void GLWindow::initGLFW()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
		printf("[ERROR] initialize glfw failed!\n");
        exit(EXIT_FAILURE);
	}

    window = glfwCreateWindow(width, height, "PRT Demo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);
}

void GLWindow::initGL()
{
	/* Enable smooth shading */
	glShadeModel( GL_SMOOTH );
	/* Set the background black */
	glClearColor( 0.1f, 0.1f, 0.1f, 0.0f );
	/* Depth buffer setup */
	glClearDepth( 1.0f );
	/* Enables Depth Testing */
	glEnable( GL_DEPTH_TEST );
	/* The Type Of Depth Test To Do */
	glDepthFunc( GL_LEQUAL );
	/* Really Nice Perspective Calculations */
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	modelMatrix = mat4(1.0f);
	viewMatrix = glm::lookAt(vec3(0.0f,0.0f,10.0f), vec3(0.0f,0.0f,-5.0f), vec3(0.0f,1.0f,0.0f));

	//model.LoadModelFromFile("assets/model/bunny.obj");
}

void GLWindow::compileShader()
{

	if( ! prog.compileShaderFromFile("shader/without_texture.vert",GLSLShader::VERTEX) )
	{
		printf("Vertex shader failed to compile!\n%s",
			prog.log().c_str());
		exit(1);
	}
	if( ! prog.compileShaderFromFile("shader/without_texture.frag",GLSLShader::FRAGMENT))
	{
		printf("Fragment shader failed to compile!\n%s",
			prog.log().c_str());
		exit(1);
	}

	prog.bindAttribLocation(0, "VertexPosition");
	prog.bindAttribLocation(1, "VertexNormal");

	if( ! prog.link() )
	{
		printf("Shader program failed to link!\n%s",
			prog.log().c_str());
		exit(1);
	}
	if( ! prog.validate() )
	{
		printf("Program failed to validate!\n%s",
			prog.log().c_str());
		exit(1);
	}
	prog.use();
}

void GLWindow::changeMatrics()
{
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(10.f), vec3(0.0f,1.0f,0.0f));
	mat4 mv = viewMatrix * modelMatrix;
	glfwGetFramebufferSize(window, &width, &height);
	mat4 projection = glm::perspective(45.0f, float(width)/height, 0.1f, 1000.0f);
	prog.setUniform("MVP", projection * mv);
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix",mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
}

void GLWindow::setUniform()
{

	glfwGetFramebufferSize(window, &width, &height);
	mat4 projection = glm::perspective(45.0f, float(width)/height, 0.1f,1000.0f);
	mat4 mv = viewMatrix * modelMatrix;

	prog.setUniform("Kd", 0.0f, 0.9f, 0.9f);
	prog.setUniform("Ld", 1.0f, 0.5f, 0.5f);
	prog.setUniform("LightPosition", viewMatrix * vec4(-5.0f,220.0f,215.0f,1.0f) );
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix",mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
	prog.setUniform("MVP", projection * mv);
}


void GLWindow::renderGL()
{
	while (!glfwWindowShouldClose(window)) {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);

		if (bAnim) changeMatrics();
		model.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//========================================================================
// Print errors
//========================================================================

void GLWindow::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

//========================================================================
// Handle key strokes
//========================================================================
void GLWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS) {
			cout << "set animation\n" << endl;
			bAnim = !bAnim;
		}
		break;
	}
}

//========================================================================
// Callback function for mouse button events
//========================================================================
void GLWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        locked = true;
    }
    else {
        locked = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

//========================================================================
// Callback function for cursor motion events
//========================================================================
void GLWindow::cursor_position_callback(GLFWwindow* window, double x, double y)
{
    if (locked)
    {
        alpha += (GLfloat) (x - cursorX) / 10.f;
        beta += (GLfloat) (y - cursorY) / 10.f;
    }

    cursorX = (int) x;
    cursorY = (int) y;
}

//========================================================================
// Callback function for scroll events
//========================================================================
void GLWindow::scroll_callback(GLFWwindow* window, double x, double y)
{
    zoom += (float) y / 4.f;
    if (zoom < 0)
        zoom = 0;
}


//========================================================================
// Callback function for framebuffer resize events
//========================================================================
void GLWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float ratio = 1.f;

    if (height > 0)
        ratio = (float) width / (float) height;

    // Setup viewport
    glViewport(0, 0, width, height);

    // Change to the projection matrix and set our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ratio, 0.1f, 1000.0f);
	//glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_DEPTH_TEST);
}
