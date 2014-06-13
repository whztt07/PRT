#ifndef _GLSETTING_H_
#define _GLSETTING_H_
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaderprogram.h"
#include "textfile.h"

using namespace std;

float fTranslate= 0.0f;
float fRotate	= 0.0f;
float fScale    = 1.0f;	// set inital scale value to 1.0f
float fDistance = 0.2f;

bool bAnim = false;

float eye[] = {0, 0, 3};
float center[] = {0, 0, -5};
float up[] = {0, 1, 0};

GLfloat alpha = 210.f, beta = -70.f;
GLfloat zoom = 2.f;
GLboolean locked = GL_FALSE;
int cursorX;
int cursorY;

// shader
GLuint vShader, fShader;
GLuint vaoHandle;

//========================================================================
// Print errors
//========================================================================

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

//========================================================================
// Handle key strokes
//========================================================================
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        locked = GL_TRUE;
    }
    else
    {
        locked = GL_FALSE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

//========================================================================
// Callback function for cursor motion events
//========================================================================
void cursor_position_callback(GLFWwindow* window, double x, double y)
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
void scroll_callback(GLFWwindow* window, double x, double y)
{
    zoom += (float) y / 4.f;
    if (zoom < 0)
        zoom = 0;
}


//========================================================================
// Callback function for framebuffer resize events
//========================================================================
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float ratio = 1.f;

    if (height > 0)
        ratio = (float) width / (float) height;

    // Setup viewport
    glViewport(0, 0, width, height);

    // Change to the projection matrix and set our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ratio, 1.0f, 1024.0f);
	//glMatrixMode(GL_MODELVIEW);
	//glEnable(GL_DEPTH_TEST);
}

void initShader(const char *VShaderFile, const char *FShaderFile)
{
	// check OpenGL, GLSL version
    //const GLubyte *renderer = glGetString( GL_RENDERER );  
    //const GLubyte *vendor = glGetString( GL_VENDOR );  
    //const GLubyte *version = glGetString( GL_VERSION );  
    //const GLubyte *glslVersion =   
    //    glGetString( GL_SHADING_LANGUAGE_VERSION );  
    //GLint major, minor;  
    //glGetIntegerv(GL_MAJOR_VERSION, &major);  
    //glGetIntegerv(GL_MINOR_VERSION, &minor);  
    //cout << "GL Vendor    :" << vendor << endl;  
    //cout << "GL Renderer  : " << renderer << endl;  
    //cout << "GL Version (string)  : " << version << endl;  
    //cout << "GL Version (integer) : " << major << "." << minor << endl;  
    //cout << "GLSL Version : " << glslVersion << endl;   
  
    vShader = glCreateShader(GL_VERTEX_SHADER);  
    if (0 == vShader) {  
        cerr << "ERROR : Create vertex shader failed" << endl;  
        exit(1);  
    }  
	
    const GLchar *vShaderCode = textFileRead(VShaderFile);  
    const GLchar *vCodeArray[1] = {vShaderCode};  
    glShaderSource(vShader,1,vCodeArray,NULL);  
  
    glCompileShader(vShader);  
    GLint compileResult;  
    glGetShaderiv(vShader,GL_COMPILE_STATUS,&compileResult);  
    if (GL_FALSE == compileResult) {  
        GLint logLen;  
        glGetShaderiv(vShader,GL_INFO_LOG_LENGTH,&logLen);  
        if (logLen > 0) {  
            char *log = (char *)malloc(logLen);  
            GLsizei written;  
            glGetShaderInfoLog(vShader,logLen,&written,log);  
            cerr << "vertex shader compile log : " << endl;  
            cerr << log << endl;  
            free(log);
        }  
    }  
  
    fShader = glCreateShader(GL_FRAGMENT_SHADER);  
    if (0 == fShader) {  
        cerr << "ERROR : Create fragment shader failed" << endl;  
        exit(1);  
    }  
    const GLchar *fShaderCode = textFileRead(FShaderFile);  
    const GLchar *fCodeArray[1] = {fShaderCode};  
    glShaderSource(fShader,1,fCodeArray,NULL);  
  
    glCompileShader(fShader);  
    glGetShaderiv(fShader,GL_COMPILE_STATUS,&compileResult);  
    if (GL_FALSE == compileResult) {  
        GLint logLen;  
        glGetShaderiv(fShader,GL_INFO_LOG_LENGTH,&logLen);  
        if (logLen > 0) {  
            char *log = (char *)malloc(logLen);  
            GLsizei written;  
            glGetShaderInfoLog(fShader,logLen,&written,log);  
            cerr << "fragment shader compile log : " << endl;  
            cerr << log << endl;  
            free(log); 
        }  
    }  
  
    GLuint programHandle = glCreateProgram();  
    if (!programHandle) {  
        cerr << "ERROR : create program failed" << endl;  
        exit(1);  
    }  
    //将着色器程序链接到所创建的程序中  
    glAttachShader(programHandle,vShader);  
    glAttachShader(programHandle,fShader);  
    //将这些对象链接成一个可执行程序  
    glLinkProgram(programHandle);  
    //查询链接的结果  
    GLint linkStatus;  
    glGetProgramiv(programHandle,GL_LINK_STATUS,&linkStatus);  
    if (GL_FALSE == linkStatus) {  
        cerr << "ERROR : link shader program failed" << endl;  
        GLint logLen;  
        glGetProgramiv(programHandle,GL_INFO_LOG_LENGTH, &logLen);  
        if (logLen > 0) {  
            char *log = (char *)malloc(logLen);  
            GLsizei written;  
            glGetProgramInfoLog(programHandle,logLen, &written,log);  
            cerr << "Program log : " << endl;  
            cerr << log << endl;  
        }
		return;
    }
    glUseProgram(programHandle);
}

void initVBO(float* positionData, float* colorData)
{
	// Create and populate the buffer objects  
    GLuint vboHandles[2];  
    glGenBuffers(2, vboHandles);  
    GLuint positionBufferHandle = vboHandles[0];  
    GLuint colorBufferHandle    = vboHandles[1];  
  
    // populate the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);  
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);  
  
    // populate the color buffer
    glBindBuffer(GL_ARRAY_BUFFER,colorBufferHandle);  
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);  
	
	// create and set-up the vertex array object
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);  
	
	// enable the vertex attribute arrays
    glEnableVertexAttribArray(0);		// vertex position  
    glEnableVertexAttribArray(1);		// vertex color
  
    // Map index 0 to the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);  
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );  
  
	// Map index 1 to the color buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);  
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );  
}



void initGLFW(GLFWwindow*& window)
{
	double t, dt_total, t_old;
    int width, height;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
		printf("[ERROR] initialize glfw failed!\n");
        exit(EXIT_FAILURE);
	}

    window = glfwCreateWindow(640, 480, "Wave Simulation", NULL, NULL);
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

    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);
}

void init_opengl(void)
{
    // Use Gouraud (smooth) shading
    glShadeModel(GL_SMOOTH);

    // Switch on the z-buffer
    glEnable(GL_DEPTH_TEST);

    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    //glVertexPointer(3, GL_FLOAT, sizeof(struct Vertex), vertex);
    //glColorPointer(3, GL_FLOAT, sizeof(struct Vertex), &vertex[0].r); // Pointer to the first color

    //glPointSize(2.0);

	// background
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
}

bool initGL()
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
	return true;
}

void compileShader(ShaderProgram& prog)
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

ShaderProgram prog;
glm::mat4 viewMatrix, modelMatrix;

void changeMatrics()
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(6.0f), vec3(0.0f,1.0f,0.0f));
	mat4 mv = viewMatrix * modelMatrix;
	mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	prog.setUniform("MVP", projection * mv);
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix",mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
}

#endif