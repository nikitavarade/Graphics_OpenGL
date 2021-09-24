#include<GL/freeglut.h>

bool bIsFullScreen = false;

GLfloat angle_t = 0.0f, angle_c = 0.0f;

int main(int argc, char *argv[])
{
	//function declarations
	void initialize(void);
	void uninitialize(void);
	void reshape(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void Update(void);
	

	//code
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("3D Animation Using Free GLUT : Nikita Varade");

	initialize();

	//callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(Update);
	glutCloseFunc(uninitialize);
	glutMainLoop();
	return(0);
}

void initialize(void)
{
	//code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		
}

void uninitialize(void)
{
	//code
}

void reshape(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{
	void UpdateC(void);
	void UpdateT(void);
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-1.5f, 0.0f, -6.0f);
	glRotatef(angle_t, 0.0f, 1.0f, 0.0f);
	UpdateT();
	glLoadIdentity();
	glTranslatef(1.5f, 0.0f, -7.0f);
	glRotatef(angle_c, 1.0f, 1.0f, 1.0f);
	UpdateC();

	glutSwapBuffers();
}

void UpdateT(void)
{
	glLineWidth(4.0f);
	glBegin(GL_TRIANGLES);

	//Front 
	glColor3f(1.0f, 0.0f, 0.0f);//R
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);//G
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glColor3f(0.0f, 0.0f, 1.0f);//B
	glVertex3f(1.0f, -1.0f, 1.0f);

	//Right 
	glColor3f(1.0f, 0.0f, 0.0f);//R
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);//B
	glVertex3f(1.0f, -1.0f, 1.0f);

	glColor3f(0.0f, 1.0f, 0.0f);//G
	glVertex3f(1.0f, -1.0f, -1.0f);

	//Back
	glColor3f(1.0f, 0.0f, 0.0f);//R
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);//G
	glVertex3f(1.0f, -1.0f, -1.0f);

	glColor3f(0.0f, 0.0f, 1.0f);//B
	glVertex3f(-1.0f, -1.0f, -1.0f);

	//Left
	glColor3f(1.0f, 0.0f, 0.0f);//R
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);//B
	glVertex3f(-1.0f, -1.0f, -1.0f);


	glColor3f(0.0f, 1.0f, 0.0f);//G
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();
	
	angle_t = angle_t + 0.05f;
	if (angle_t > 360.0f)
	{
		angle_t = 0.0f;
	}

}

void UpdateC(void)
{
	glLineWidth(4.0f);
	glBegin(GL_QUADS);

	//Front face
	glColor3f(1.0f, 0.0f, 0.0f);//R 
	glVertex3f(1.0f, 1.0f, 1.0f);//FRT
	glVertex3f(-1.0f, 1.0f, 1.0f);//FLT
	glVertex3f(-1.0f, -1.0f, 1.0f);//FLB
	glVertex3f(1.0f, -1.0f, 1.0f);//FRB

								  //Left Face
	glColor3f(0.0f, 1.0f, 0.0f);//B 
	glVertex3f(1.0f, 1.0f, 1.0f);//LT
	glVertex3f(1.0f, -1.0f, 1.0f);//LB
	glVertex3f(1.0f, -1.0f, -1.0f);//RB
	glVertex3f(1.0f, 1.0f, -1.0f);//RT

								  //Back Face
	glColor3f(0.0f, 0.0f, 1.0f);//G
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	//Right Face
	glColor3f(1.0f, 1.0f, 0.0f);//Y
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	//Top Face
	glColor3f(0.0f, 1.0f, 1.0f);//C
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	//Bottom face
	glColor3f(1.0f, 0.0f, 1.0f);//M
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glEnd();
	
	angle_c = angle_c + 0.05f;
	if (angle_c > 360.0f)
	{
		angle_c = 0.0f;
	}

}

void keyboard(unsigned char key, int x, int y)
{
	//code
	switch (key)
	{

	case 27:
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
		if (bIsFullScreen == false)
		{
			glutFullScreen();
			bIsFullScreen = true;
		}
		else
		{
			glutLeaveFullScreen();
			bIsFullScreen = false;
		}
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	//code
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		break;

	case GLUT_RIGHT_BUTTON:
		glutLeaveMainLoop();
		break;
	}
}

void Update()
{

	angle_c = angle_c + 0.01f;
	if (angle_c > 360.0f)
	{
		angle_c = 0.0f;
	}
	glutPostRedisplay();

}