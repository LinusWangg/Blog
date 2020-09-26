#include <stdlib.h>
#include<stdio.h>
#include<windows.h>
#include <GL/glut.h>
float l1 = 0.25;
float ll = 1.0;

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

	GLfloat lightpos[] = { 0.f, 100.f, -450.f, ll };			// ø……Ë∂Øª≠
	GLfloat light0_mat1[] = { 0.2, 0.2, 0.2, 0.3f };
	GLfloat light0_diff[] = { 1.0, 1.0, 1.0, 0.3 };
	GLfloat lightpos2[] = { 0.0,0.0,0.0,ll };
	GLfloat light1_mat1[] = { 0.1,0.1,0.1,0.3 };
	GLfloat light1_diff[] = { 0.1,0.1,0.1,0.3 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_mat1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff);
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

	glColor3f(0.5, 0.6, 0.6);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(200.0, 100.0, 0.0);
	glVertex3f(-200.0, 100.0, 0.0);
	glVertex3f(-200.0, -100.0, 0.0);
	glVertex3f(200.0, -100.0, 0.0);
	glEnd();

	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(200.0, -100.0, 300.0);
	glVertex3f(200.0, -100.0, 0.0);
	glVertex3f(-200.0, -100.0, 0.0);
	glVertex3f(-200.0, -100.0, 300.0);
	glEnd();

	glColor3f(0.7, 0.7, 0.9);
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(200.0, 100.0, 300.0);
	glVertex3f(-200.0, 100.0, 300.0);
	glVertex3f(-200.0, 100.0, 0.0);
	glVertex3f(200.0, 100.0, 0.0);
	glEnd();

	glColor3f(0.5, 0.6, 0.6);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(-200.0, -100.0, 300.0);
	glVertex3f(-200.0, -100.0, 0.0);
	glVertex3f(-200.0, 100.0, 0.0);
	glVertex3f(-200.0, 100.0, 300.0);
	glEnd();

	glColor3f(0.5, 0.6, 0.6);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(200.0, -100.0, 300.0);
	glVertex3f(200.0, 100.0, 300.0);
	glVertex3f(200.0, 100.0, 0.0);
	glVertex3f(200.0, -100.0, 0.0);
	glEnd();
	//bed
	glColor3f(0.7, 0.7, 0.5);
	glTranslatef(100.0, -60.0, 10.0);//(100,-60,-290)
	glPushMatrix();
	glScalef(6.5, 4.0, 1.0);
	glutSolidCube(20.0);
	glPopMatrix();
	glTranslatef(0.0, -25.0, 120);//(100,-85,-170)
	glPushMatrix();
	glScalef(6.0, 1.5, 12.0);
	glutSolidCube(20.0);
	glPopMatrix();
	glTranslated(0, 20, 40.0);//(100,-65,-130)
	glPushMatrix();
	glScalef(5.0, 0.5, 5.0);
	glutSolidCube(20.0);
	glPopMatrix();
	glTranslatef(0.0, 2.0, -105.0);//(100,-63,-235)
	glPushMatrix();
	glScalef(3.0, 0.7, 1.5);
	glutSolidCube(20.0);
	glPopMatrix();
	//air-condition

	glTranslatef(-100.0, 63.0, -75.0);
	glPushMatrix();
	glTranslatef(-100.0, 60.0, 50.0);
	glPushMatrix();
	glScalef(5.0, 1.5, 2.5);
	glutSolidCube(20.0);
	glPopMatrix();


	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-200., 200., -100., 100., 300., 800.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();
}