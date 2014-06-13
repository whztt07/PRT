// glutEx1.cpp : 定义控制台应用程序的入口点。
//

#include <stdlib.h>
#include <glm\gtc\matrix_transform.hpp>
#include "prt.h"
#include "utils.h"
#include "glSetting.h"
#include "CAssimpModel.h"
using namespace std;

// PRT
Color light;
Color** coeffs;
Scene scene;
const int bands = 5;
const int numSamples = 5;

//顶点位置数组  
float positionData[] = {  
    -0.8f, -0.8f, 0.0f,  
    0.8f, -0.8f, 0.0f,  
    0.0f,  0.8f, 0.0f };  
//颜色数组  
float colorData[] = {  
        1.0f, 0.0f, 0.0f,  
        0.0f, 1.0f, 0.0f,  
        0.0f, 0.0f, 1.0f }; 

void Render(Color* light, Color** coeffs, Scene* scene, int bands)
{
	//glPushMatrix();
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < scene->number_of_triangles; i++)
	{
		Triangle& t = scene->triangles[i];
		Vector3& v0 = scene->vertices[t.a];
		Vector3& v1 = scene->vertices[t.b];
		Vector3& v2 = scene->vertices[t.c];
		Color c0 (0.0f, 0.0f, 0.0f);
		Color c1 (0.0f, 0.0f, 0.0f);
		Color c2 (0.0f, 0.0f, 0.0f);
		for (int k = 0; k < bands*bands; k++)
		{
			c0.r += (light[k].r * coeffs[t.a][k].r);
			c0.g += (light[k].g * coeffs[t.a][k].g);
			c0.b += (light[k].b * coeffs[t.a][k].b);
			c1.r += (light[k].r * coeffs[t.b][k].r);
			c1.g += (light[k].g * coeffs[t.b][k].g);
			c1.b += (light[k].b * coeffs[t.b][k].b);
			c2.r += (light[k].r * coeffs[t.c][k].r);
			c2.g += (light[k].g * coeffs[t.c][k].g);
			c2.b += (light[k].b * coeffs[t.c][k].b);
		}
		glColor3f(c0.r, c0.g, c0.b);
		glVertex3f(v0.x, v0.y, v0.z);
		glColor3f(c1.r, c1.g, c1.b);
		glVertex3f(v1.x, v1.y, v1.z);
		glColor3f(c2.r, c2.g, c2.b);
		glVertex3f(v2.x, v2.y, v2.z);
	}
	glEnd();
	//glPopMatrix();
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.f, 0.f, 3.f, 0.f, 0.f, -5.f, 0.f, 1.f, 0.f);
	//gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	//glTranslatef(0.f, 0.f, -zoom);
	//glRotatef(beta, 1.f, 0.f, 0.f);
	//glRotatef(beta, 0.f, 0.f, 1.f);

	glRotatef(fRotate, 0.f, 1.f, 0.f);
	glScalef(fScale, fScale, fScale);
	//glTranslatef(-scene.center.x, -scene.center.y, -scene.center.z);

	Render(&light, coeffs, &scene, bands);
	//glutSwapBuffers();
	if (bAnim) {
		fRotate += 0.5f;
		if (fRotate > 360)
			fRotate -= 360;
	}
}

// test!
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3);  
    glBindVertexArray(0);
}

void renderModel(CAssimpModel& model)
{
	static GLint Frames = 0;
	// Clear the color and depth buffers.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	// We don't want to modify the projection matrix. */
	glMatrixMode( GL_MODELVIEW );

	if (bAnim)
		changeMatrics();
	model.render();
}

int main (int argc,  char *argv[])
{
	// load resources
	//srand(time(NULL));
	//cout << "Loading resources\n";
	//cout << "   Loading Light probe...\n";
	//Image img;			// image for light probe				
	//img.loadFromFile("uffizi_probe.jpg");
	//cout << "  Loading Model...\n";
	//scene.loadMeshFromFile("models/Head_Sad.x");			// get mesh
	//cout << "     # of vertices  = " << scene.number_of_vertices << endl;
	//cout << "     # of triangles = " << scene.number_of_triangles << endl;
	//Color white(1, 1, 1);
	//Color black(0, 0, 0);
	//Color grey(0.5f, 0.5f, 0.5f);
	//scene.albedo = &white;
	//for (int i = 0; i < scene.number_of_vertices; i++)
	//	scene.material[i] = 0;
	//fScale = scene.aabb.max.x - scene.aabb.min.x;
	//fScale = aisgl_max(scene.aabb.max.y - scene.aabb.min.y, fScale);
	//fScale = aisgl_max(scene.aabb.max.z - scene.aabb.min.z, fScale);
	//fScale = 1.f/fScale * 2;
	//
	//coeffs = new Color*[scene.number_of_vertices];
	//for (int i = 0; i < scene.number_of_vertices; i++)
	//	coeffs[i] = new Color[bands*bands];

	//cout << "Preprocess\n";
	//auto startTime = Clock::now();

	//Sampler sampler;
	//cout << "  Generate Samples...\n";
	//GenerateSamples(&sampler, numSamples);

	//cout << "  Precompute SH Functions...\n";
	//PrecomputeSHFunctions(&sampler, bands);

	//cout << "  Project Light Function...\n";
	//ProjectLightFunction(&light, &sampler, &img, bands);	// get light

	//cout << "  Project Unshadowed...\n";
	//ProjectUnshadowed(coeffs, &sampler, &scene, bands);	// get coeffs
	////ProjectShadowed(coeffs, &sampler, &scene, bands);

	//auto endTime = Clock::now();
	//auto ms = duration_cast<milliseconds>(endTime-startTime).count();
	//cout << "Preproc time: " << ms << " ms" << '\n';

	
	// PART 2 Runtime

	// Initialize GLFW
	GLFWwindow* window;
	initGLFW(window);
	// Initialize GLEW
	if (GLEW_OK != glewInit()) {
		printf("[ERROR] Failed to initialize GLEW\n");
		exit(EXIT_FAILURE);
	}

	// Use Gouraud (smooth) shading
    //glShadeModel(GL_SMOOTH);
    // Switch on the z-buffer
    //glEnable(GL_DEPTH_TEST);
	// background
	//glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	//initShader("shader/basic.vert", "shader/basic.frag");
	//initShader("shader/without_texture.vert", "shader/without_texture.frag");
	//initVBO(positionData, colorData);
	
	//ShaderProgram prog;
	CAssimpModel model(prog);
	model.LoadModelFromFile("models/happy_vrip_res2.obj");
	initGL();
	compileShader(prog);
	//glm::mat4 modelMatrix = glm::mat4(1.0f);
	//glm::mat4 viewMatrix = glm::lookAt(vec3(0.0f,0.0f,10.0f), vec3(0.0f,0.0f,-5.0f), vec3(0.0f,1.0f,0.0f));
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(vec3(0.0f,0.0f,10.0f), vec3(0.0f,0.0f,-5.0f), vec3(0.0f,1.0f,0.0f));
	//setUniform();
	mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f,1000.0f);
	mat4 mv = viewMatrix * modelMatrix;

	prog.setUniform("Kd", 0.0f, 0.9f, 0.9f);
	prog.setUniform("Ld", 1.0f, 0.5f, 0.5f);
	prog.setUniform("LightPosition", viewMatrix * vec4(-5.0f,220.0f,215.0f,1.0f) );
	prog.setUniform("ModelViewMatrix", mv);
	prog.setUniform("NormalMatrix",mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
	prog.setUniform("MVP", projection * mv);

	while (!glfwWindowShouldClose(window)){
		//redraw();
		//display();
		renderModel(model);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
