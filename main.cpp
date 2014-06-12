// glutEx1.cpp : 定义控制台应用程序的入口点。
//

#include <stdlib.h>
#include "glut.h"
#include "prt.h"
#include <opencv2\opencv.hpp>
#include "utils.h"
#include "GLCamera.h"
#include "control.h"
using namespace std;
using namespace cv;

GLCamera* camera;

Color light;
Color** coeffs;
Scene scene;
const int bands = 3;
int scale = 2;

void Render(Color* light, Color** coeffs, Scene* scene, int bands)
{
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
}

void redraw()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f(1.f, 1.f, 1.f);
	



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// scale the whole asset to fit into our view frustum 
	//tmp = scene_max.x-scene_min.x;
	//tmp = aisgl_max(scene_max.y - scene_min.y,tmp);
	//tmp = aisgl_max(scene_max.z - scene_min.z,tmp);
	//tmp = 1.f / tmp;
	float tmp = 10;
	glTranslatef(0.f, 0.f, 0.f);
	glScalef(tmp, tmp, tmp);
	//gluLookAt(0.f,0.f,3.f,0.f,0.f,-5.f,0.f,1.f,0.f);
	//glMatrixMode(GL_MODELVIEW);
	//LookAt();
	//glLoadIdentity();
	//camera->setModelViewMatrix();
	Render(&light, coeffs, &scene, bands);
	//glBegin(GL_QUADS);
	//glColor3f(1, 0, 0);
	//glVertex3f(-0.5, 0.5, 0.5);
	//glVertex3f(0.5, 0.5, 0.5);
	//glVertex3f(0.5, -0.5, 0.5);
	//glVertex3f(-0.5, -0.5, 0.5);
	//glEnd(); // GL_QUADS
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	srand(time(NULL));
	cout << "Loading image..." << endl;

	Mat mat = imread("uffizi_probe.jpg");
	if (mat.empty()) {
		fprintf(stderr, "Error: load image failed.");
		return -1;
	}
	namedWindow("image", CV_WINDOW_AUTOSIZE);
	imshow("image", mat);
	//   waitKey();

	Image img;
	img.height = mat.rows;
	img.width = mat.cols;
	img.pixel[0] = new uchar[img.height * img.width];
	img.pixel[1] = new uchar[img.height * img.width];
	img.pixel[2] = new uchar[img.height * img.width];
	for (int i = 0; i < img.height; i++) {
		for (int j = 0; j < img.width; j++) {
			Vec3b intensity = mat.at<Vec3b>(i, j);
			img.pixel[0][i*img.height + j] = intensity.val[2];
			img.pixel[1][i*img.width + j] = intensity.val[1];
			img.pixel[2][i*img.width + j] = intensity.val[0];
		}
	}

	cout << "PRT start\n";

	// 1
	//const int bands = 3;	// -> global
	Sampler sampler;
	cout << "  Generate Samples...\n";
	GenerateSamples(&sampler, 10);			// N = 10
	cout << "  Precompute SH Functions...\n";
	PrecomputeSHFunctions(&sampler, bands);		// bands = 3

	//Scene scene;	/// -> global;
	//Color light;	// -> global
	cout << "  Project Light Function...\n";
	ProjectLightFunction(&light, &sampler, &img, bands);	// get light

	cout << "  Loading Model...\n";
	load_mesh("models/ChessKing.obj", &scene);

	cout << "     # of vertices  = " << scene.number_of_vertices << endl;
	cout << "     # of triangles = " << scene.number_of_triangles << endl;
	Color white(1, 1, 1);
	Color black(0, 0, 0);
	scene.albedo = &white;
	for (int i = 0; i < scene.number_of_vertices; i++)
		scene.material[i] = 0;

	coeffs = new Color*[scene.number_of_vertices];	// 2
	for (int i = 0; i < scene.number_of_vertices; i++)
		coeffs[i] = new Color[bands*bands];

	cout << "  Project Unshadowed...\n";
	ProjectUnshadowed(coeffs, &sampler, &scene, bands);	// get coeffs

	glutInitWindowSize(900,600);
	glutInitWindowPosition(100,100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	int windowHandle = glutCreateWindow("PRT");
	//glutMouseFunc(processMouse);
	///     glutKeyboardFunc(KeyFunc);    键盘按键
	glutMouseFunc(MouseFunc);   
	glutMotionFunc(MouseMotion);
	//Vector3d pos(0.0f, 0.0f, 12.0f);
	//Vector3d target(0.f, 0.f, 0.f);
	//Vector3d up(0.f, 1.f, 0.f);
	//camera = new GLCamera(pos, target, up);

	glutDisplayFunc(redraw);

	glutMainLoop();


	//delete camera;
	return 0;
}


