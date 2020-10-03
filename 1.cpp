#include <stdlib.h>
#include<stdio.h>
#include<windows.h>
#include <GL/glut.h>
#include<math.h>
float l1 = 0.25;
float ll = 1.0;
float light = 0.0;
float angle = 0.0;
int option = 0;
float r = 64, g = 224, b = 208;
float windowangle = 0.0;
float rocketx = -250;

void init(void) // All Setup For OpenGL Goes Here
{
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glFrontFace(GL_CCW);
	glEnable(GL_COLOR_MATERIAL);
}

void display(void) // Here's Where We Do All The Drawing
{
	glPushMatrix();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	GLfloat lightpos[] = { 0.f, 70.f, -480.f, light };
	GLfloat light0_mat1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light0_diff[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightpos2[] = { -195.0,0.0,-400.0,ll };
	GLfloat light1_mat1[] = { 0.0, 0.0, 0.0, 0.0f };
	GLfloat light1_diff[] = { 1.0,1.0,1.0,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_mat1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_mat1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diff);

	GLfloat no_mat[] = { 0.0,0.0,0.0,1.0 };
	GLfloat mat_ambient[] = { 0.2,0.2,0.2,1.0 };
	GLfloat mat_diffuse[] = { 0.9,0.9,0.9,1.0 };
	GLfloat mat_specular[] = { 0.3,0.3,0.3,1.0 };
	GLfloat high_shininess[] = { 20.0 };
	GLfloat high_mat[] = { l1,l1,l1,1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, high_mat);

	glTranslatef(0.0, 0.0, -300.0);
	glPushMatrix();
	static GLfloat wall_mat[] = { 1.f, 1.f, 1.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_mat);
	//back
	glColor3ub(255, 228, 196);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(200.0, 100.0, -300.0);
	glVertex3f(-200.0, 100.0, -300.0);
	glVertex3f(-200.0, -100.0, -300.0);
	glVertex3f(200.0, -100.0, -300.0);
	glEnd();
	//bot
	glColor3ub(255, 165, 0);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(200.0, -100.0, 0.0);
	glVertex3f(200.0, -100.0, -300.0);
	glVertex3f(-200.0, -100.0, -300.0);
	glVertex3f(-200.0, -100.0, 0.0);
	glEnd();
	//top
	glColor3ub(224,255,255);
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(200.0, 100.0, 0.0);
	glVertex3f(-200.0, 100.0, 0.0);
	glVertex3f(-200.0, 100.0, -300.0);
	glVertex3f(200.0, 100.0, -300.0);
	glEnd();
	//left
	glColor3ub(255, 228, 196);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(-200.0, -100.0, 0.0);
	glVertex3f(-200.0, -100.0, -300.0);
	glVertex3f(-200.0, 100.0, -300.0);
	glVertex3f(-200.0, 100.0, 0.0);
	glEnd();
	//right
	//glColor3ub(135, 206 ,235);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(200.0, -100.0, 0.0);
	glVertex3f(200.0, 100.0, 0.0);
	glVertex3f(200.0, 100.0, -300.0);
	glVertex3f(200.0, -100.0, -300.0);
	glEnd();
	//bed

	glColor3ub(255,228,225);
	glPushMatrix();
	glTranslatef(100.0, -60.0, -290.0);//(100,-60,-290)
	glScalef(6.5, 4.0, 1.0);
	glutSolidCube(20.0);
	glPopMatrix();
	glColor3ub(190, 190, 190);
	glPushMatrix();
	glTranslatef(100.0, -85.0, -170);//(100,-85,-170)
	glScalef(6.0, 1.5, 12.0);
	glutSolidCube(20.0);
	glPopMatrix();
	glColor3ub(224, 255, 255);
	glPushMatrix();
	glTranslated(100, -65, -130);//(100,-65,-130)
	glScalef(5.0, 0.5, 5.0);
	glutSolidCube(20.0);
	glPopMatrix();
	glColor3ub(135, 206, 250);
	glPushMatrix();
	glTranslatef(100, -63, -235);//(100,-63,-235)
	glScalef(3.0, 0.7, 1.5);
	glutSolidCube(20.0);
	glPopMatrix();

	//air-condition
	glColor3ub(224,255, 255);
	glPushMatrix();
	glTranslatef(-100.0, 60.0, -275.0); //(-100,60,-275)
	glScalef(5.0, 1.5, 2.5);
	glutSolidCube(20.0);
	glPopMatrix();

	//Table
	glColor3ub(255,255,224);
	glPushMatrix();
	glTranslatef(-100, -30, -260);  //(-100,-30,-260)
	glScalef(8.0, 0.4, 6.5);
	glutSolidCube(20.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-175, -67, -260);  //(-175,-67,-260)
	glScalef(0.5, 3.3, 6.5);
	glutSolidCube(20.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-40, -67, -260);  //(-40,-67,-260)
	glScalef(2.0, 3.3, 6.5);
	glutSolidCube(20.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-145, -70, -285);  //(-145,-70,-285)
	glScalef(3.0, 0.4, 4.0);
	glutSolidCube(20.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-119, -50, -285);  //(-119,-50,-285)
	glScalef(0.4, 2.0, 4.0);
	glutSolidCube(20.0);
	glPopMatrix();

	//chair
	glColor3ub(176 ,196, 222);
	glPushMatrix();
	glTranslatef(-100, -60, -80);  //(-100,-60,-80)
	glRotatef(angle, 0.0, 1.0, 0.0);
	glScalef(2.5, 0.3, 2.5);
	glutSolidCube(20.0);
	glPopMatrix();
	glPushMatrix();
	glColor3ub(255,255,255);
	glTranslatef(-100, -80, -80);  //(-100,-80,-80)
	glScalef(0.2, 1.8, 0.2);
	glutSolidCube(20.0);
	glPopMatrix();
	glColor3ub(105,105,105);
	glPushMatrix();
	glTranslatef(-100, -97, -80);  //(-100,-97,-80)
	GLUquadric* pObj;
	pObj = gluNewQuadric();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glPushMatrix();
	gluCylinder(pObj, 30, 30, 6, 32, 5);
	//glScalef(1.5, 0.3, 1.5);
	//glutSolidCube(20.0);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-100, -100, -80);  //(-100,-100,-80)
	glRotatef(90.0, 1.0, 0.0, 0.0);
	int i;
	glBegin(GL_POLYGON);
	for (i = 0; i < 100; i++)
	{
		glVertex2f(30 * cos(2 * 3.1415926536 / 100 * i), 30 * sin(2 * 3.1415926536 / 100 * i));
	}
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-100, -97, -80);  //(-100,-97,-80)
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (i = 0; i < 100; i++)
	{
		glVertex2f(30 * cos(2 * 3.1415926536 / 100 * i), 30 * sin(2 * 3.1415926536 / 100 * i));
	}
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glColor3ub(176, 196, 222);
	glTranslatef(-100, -60, -80);  //(-100,-60,-80)
	glRotatef(angle, 0.0, 1.0, 0.0);
	glTranslatef(0, 23, 25);  //(-100, -37, -55)
	glScalef(2.5, 2.5, 0.3);
	glutSolidCube(20.0);
	glPopMatrix();

	//window
	glPushMatrix();
	glTranslatef(-194, -32, -120);  //(-194,-32,-120)
	glScalef(0.6, 0.2, 5.0);
	glutSolidCube(20.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-198, 0, -72);  //(-198,0,-72)
	glScalef(0.2, 3.0, 0.2);
	glutSolidCube(20.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-198, 0, -168);  //(-198,0,-168)
	glScalef(0.2, 3.0, 0.2);
	glutSolidCube(20.0);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-194, 32, -120);  //(-194,32,-120)
	glScalef(0.6, 0.2, 5.0);
	glutSolidCube(20.0);
	glPopMatrix();
	
	glColor3ub(r,g,b);
	glPushMatrix();
	glTranslatef(-195,0,-72);
	glRotatef(windowangle, 0.0, -1.0, 0.0);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(0, 31, -96);
	glVertex3f(0, 31, 0);
	glVertex3f(0, -31, 0);
	glVertex3f(0, -31, -96);
	glEnd();
	glPopMatrix();

	//light
	glColor3ub(190,190,190);
	glPushMatrix();
	glTranslatef(0, 90, -120);  //(0,90,-120)
	glScalef(0.2, 1.0, 0.2);
	glutSolidCube(20.0);
	glPopMatrix();
	glColor3ub(255,231,186);
	glPushMatrix();
	glTranslatef(0, 80, -120);  //(0,80,-120)
	GLUquadric* pObj2;
	pObj2 = gluNewQuadric();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glPushMatrix();
	gluCylinder(pObj2, 30, 30, 20, 32, 5);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	/*glutSwapBuffers();
	angle++;*/

	//rocket
	glColor3ub(255, 255, 255);
	glPushMatrix();
	glTranslatef(rocketx, 0, -120);
	glScalef(1.0, 1.0, 1.0);
	glutSolidCube(20.0);
	glPopMatrix();



	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-200., 200., -100., 100., 300., 800.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'\033':
		exit(0);
		break;
	case'1':
		printf("the chair will roteta");
		option = 1;
		break;
	case'2':
		printf("turn on the light");
		option = 2;
		break;
	case'3':
		printf("turn off the light");
		option = 3;
		break;
	case'4':
		printf("open the window");
		option = 4;
		break;
	case'5':
		printf("close the window");
		option = 5;
		break;
	case'6':
		printf("Rocket on");
		option = 6;
		break;
	case'7':
		printf("Rocket off");
		option = 7;
		break;
	default:
		printf("error\n");
	}
}

void idle()
{
	if (option == 1) {
		Sleep(5);
		angle++;
		angle = (int(angle) % 360);
	}
	if (option == 2) {
		Sleep(5);
		light = 0.7;
	}
	if (option == 3) {
		Sleep(5);
		light = 0.0;
	}
	if (option == 4) {
		Sleep(5);
		if (windowangle <= 110)
			windowangle++;
	}
	if (option == 5) {
		Sleep(5);
		if (windowangle > 0)
			windowangle--;
	}
	if (option == 6) {
		Sleep(5);
		if (windowangle >= 90 && rocketx <= -200)
			rocketx++;
		else if (rocketx > -200 && rocketx <= 180)
			rocketx++;
	}
	if (option == 7) {
		Sleep(5);
		if (windowangle >= 90 && rocketx >= -200)
			rocketx--;
		if (windowangle <= 90 && rocketx <= 200 && rocketx >= -180)
			rocketx--;
	}
	glutPostRedisplay();
}



void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(1200, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 1");

	init();

	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();
}