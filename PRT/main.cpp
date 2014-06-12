// glutEx1.cpp : 定义控制台应用程序的入口点。
//

#include <stdlib.h>
#include "glut.h"
#include "prt.h"
#include "utils.h"
#include "glSetting.h"
using namespace std;

// PRT
Color light;
Color** coeffs;
Scene scene;
const int bands = 5;
const int numSamples = 30;

void Render(Color* light, Color** coeffs, Scene* scene, int bands)
{
	glPushMatrix();
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
	glPopMatrix();
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.f, 0.f, 3.f, 0.f, 0.f, -5.f, 0.f, 1.f, 0.f);
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

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
	//getFPS();
	glutSwapBuffers();
}


int main (int argc,  char *argv[])
{
	// load resources
	srand(time(NULL));
	cout << "Loading resources\n";
	cout << "   Loading Light probe...\n";
	Image img;			// image for light probe				
	img.loadFromFile("uffizi_probe.jpg");
	cout << "  Loading Model...\n";
	scene.loadMeshFromFile("models/Head_Sad.x");			// get mesh
	cout << "     # of vertices  = " << scene.number_of_vertices << endl;
	cout << "     # of triangles = " << scene.number_of_triangles << endl;
	Color white(1, 1, 1);
	Color black(0, 0, 0);
	Color grey(0.5f, 0.5f, 0.5f);
	scene.albedo = &white;
	for (int i = 0; i < scene.number_of_vertices; i++)
		scene.material[i] = 0;
	fScale = scene.aabb.max.x - scene.aabb.min.x;
	fScale = aisgl_max(scene.aabb.max.y - scene.aabb.min.y, fScale);
	fScale = aisgl_max(scene.aabb.max.z - scene.aabb.min.z, fScale);
	fScale = 1.f/fScale * 2;
	
	coeffs = new Color*[scene.number_of_vertices];
	for (int i = 0; i < scene.number_of_vertices; i++)
		coeffs[i] = new Color[bands*bands];

	cout << "Preprocess\n";
	auto startTime = Clock::now();

	Sampler sampler;
	cout << "  Generate Samples...\n";
	GenerateSamples(&sampler, numSamples);

	cout << "  Precompute SH Functions...\n";
	PrecomputeSHFunctions(&sampler, bands);

	cout << "  Project Light Function...\n";
	ProjectLightFunction(&light, &sampler, &img, bands);	// get light

	cout << "  Project Unshadowed...\n";
	ProjectUnshadowed(coeffs, &sampler, &scene, bands);	// get coeffs
	//ProjectShadowed(coeffs, &sampler, &scene, bands);

	auto endTime = Clock::now();
	auto ms = duration_cast<milliseconds>(endTime-startTime).count();
	cout << "Preproc time: " << ms << " ms" << '\n';

	glutInitWindowSize(480, 480);
	glutInitWindowPosition(100,100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);
	glutCreateWindow("PRT");
	initGLUT();
	glutDisplayFunc(redraw);
	glutMainLoop();
	return 0;
}
