#ifndef _GLSETTING_H_
#define _GLSETTING_H_
#include <gl/glut.h>

float fTranslate= 0.0f;
float fRotate	= 0.0f;
float fScale    = 1.0f;	// set inital scale value to 1.0f
float fDistance = 0.2f;

float bunnyPosition[][2] = {
	{0, 0},		{0, 1.5},	{0, -1.5},
	{1.5, 0},	{1.5, 1.5},	{1.5, -1.5},
	{-1.5, 0},	{-1.5, 1.5},	{-1.5, -1.5}
};

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

bool bDrawList = false;
bool bShowTable = true;
bool bShowBunny = true;

int wHeight = 0;
int wWidth = 0;

float eye[] = {0, 0, 3};
float center[] = {0, 0, -5};
float up[] = {0, 1, 0};

void updateView(int width, int height)
{
	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	if (bPersp==1)
        gluPerspective(45,1,1,100);	
	else  
	    glOrtho(-3 ,3, -3, 3,-100,100);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; updateView(580,480);break; }

	case ' ': {bAnim = !bAnim; break;}
	case 'o': {bWire = !bWire; break;}

	case 'a': {eye[0]=eye[0]+fDistance;center[0]=center[0]+fDistance;
		break;
			  }
	case 'd': {eye[0]=eye[0]-fDistance;center[0]=center[0]-fDistance;
		break;
			  }
	case 'w': {eye[1]=eye[1]-fDistance;center[1]=center[1]-fDistance;
        break;
			  }
	case 's': {eye[1]=eye[1]+fDistance;center[1]=center[1]+fDistance;
		break;
			  }
	case 'z': {eye[2]=eye[2]*0.95;
		break;
			  }
	case 'c': {eye[2]=eye[2]*1.05;
		break;
			  }
	case 'l': {bDrawList = !bDrawList;	// 切换显示列表和非显示列表绘制方式
		break;
			  }
	case  '1':{bDrawList = !bDrawList; 
		break;
			  }
	case '2': {bShowTable = !bShowTable;
		break;
			  }
	case '3': {bShowBunny = !bShowBunny;
		break;
			  }
	}
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	const double aspectRatio = (float)width/height, fieldOfView = 45.0;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspectRatio, 1.0f, 1000.0f);
	glViewport(0, 0, width, height);
}

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];
	//double elapsedTime;

	//elapsedTime = timer.getElapsedTimeInMilliSec();
	char mode[64];
	if (bDrawList)
		strcpy(mode, "unShowed");
	else
		strcpy(mode, "naive");

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	//if (time - timebase > 1000) {
		//sprintf(buffer,"FPS:%4.2f %s  time:%f ms",
		//	frame*1000.0/(time-timebase), mode, elapsedTime);
		sprintf(buffer,"FPS:%4.2f %s", frame*1000.0/(time-timebase), mode);
		timebase = time;		
		frame = 0;
	//}

	//glutSetWindowTitle(buffer);
	char *c;
	glColor3f(1.f, 1.f, 1.f);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);		// 选择投影矩阵
	glPushMatrix();						// 保存原矩阵
	glLoadIdentity();					// 装入单位矩阵
	glOrtho(0,480,0,480,-1,1);			// 位置正投影
	glMatrixMode(GL_MODELVIEW);			// 选择Modelview矩阵
	glPushMatrix();						// 保存原矩阵
	glLoadIdentity();					// 装入单位矩阵*/
	glRasterPos2f(10,10);
	for (c=buffer; *c != '\0'; c++) {		
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);		// 选择投影矩阵
	glPopMatrix();						// 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);			// 选择Modelview矩阵
	glPopMatrix();						// 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);	
}

void initGLUT()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light_pos[] = {5,5,5,1};
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,white);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	//glEnable(GL_LIGHT0);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//glEnable(GL_NORMALIZE);
	////if(getenv("MODEL_IS_BROKEN"))  
	////	glFrontFace(GL_CW);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	//glutGet(GLUT_ELAPSED_TIME);

	glutIdleFunc(idle);
	glutKeyboardFunc(key);
	glutReshapeFunc(reshape);
}


#endif