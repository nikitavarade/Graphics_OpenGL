#include<stdio.h>
#include<windows.h>
#define _USE_MATH_DEFINES 1
#include<math.h>
#include<gl/GL.h>// this is for compiler
#include<gl/GL.h>// this is for compiler
#pragma comment(lib,"opengl32.lib")//this statement is for linker
#include<gl/GLU.h>// this is for compiler
#pragma comment(lib,"glu32.lib")//this statement is for linker
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

//global function declaration

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

FILE* gpFile = NULL;
HWND ghwnd;
DWORD dwStyle;
bool bFullScreen = false;
bool bLight = false;
HDC ghdc = NULL;
HGLRC ghrc = NULL;// opengl rendering concept
bool gbActiveWindow = false;

//Specify Light Configuration Parameters

GLfloat LightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat LightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat LightPosition[] = { 0.0f,0.0f,0.0f,1.0f };

GLfloat Light_Model_Ambient[] = { 0.2f,0.2f,0.2f,1.0f };
GLfloat Light_Model_Local_Viewer[] = {0.0f};

GLUquadric *quadric[24];

GLfloat angleOfXRotation = 0.0f;
GLfloat angleOfYRotation = 0.0f;
GLfloat angleOfZRotation = 0.0f;

GLint keyPressed = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function declaration
	void display(void);
	void Update(void);
	
	int initialize();
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Material 24 Sphere");
	int iRet = 0;
	bool bDone = false;

	//File Handling
	if (fopen_s(&gpFile, "Log.text", "w") != 0)
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Created"), TEXT("Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File Is Successfully Completed ");
	}

	//code
	//initialization of WNDCLASSEX

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0; //Extra Information of Class
	wndclass.cbWndExtra = 0; //Extra Information of Window
	wndclass.lpfnWndProc = WndProc; //Register Call Back
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //for OS default icon
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); // FOR OS DEFAULT CURSOR
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//register above class
	RegisterClassEx(&wndclass);

	//create window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Material 24 Sphere"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		100,
		100,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,//handle of parent
		NULL,//Handle of MenuBar  
		hInstance,
		NULL);

	ghwnd = hwnd;

	iRet = initialize();
	if (iRet == -1)
	{
		fprintf(gpFile, "Choose Pixel Format Failed\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -2)
	{
		fprintf(gpFile, "Set pixel format error\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -3)
	{
		fprintf(gpFile, " wglCreateContext Failed\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -4)
	{
		fprintf(gpFile, "MakeCurrent Error\n");
		DestroyWindow(hwnd);
	}
	else
	{
		fprintf(gpFile, "initialization succeded\n");
	}
	ShowWindow(hwnd, iCmdShow);

	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//Game Loop
	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))

		{
			if (msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == true)
			{
				//Here call update
				Update();
			}
			display();
			//here display() is getting call instead of calling in WM_PAINT
		}

	}
	return((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//code
	void ReSize(int, int);
	void uninitialize(void);
	void ToggleFullScreen(void);

	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActiveWindow = true;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = false;
		break;

	case  WM_SIZE:
		ReSize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_ERASEBKGND:// here we are adding new case WM_ERASEBKGND instead of WM_PAINT
					   //display() is getting called in game loop 
		return(0);//do not go to defWidowProc
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_KEYDOWN:
		//As it in toggle fulscreen
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		case 0x46:
			ToggleFullScreen();
			break;
		}


		break;
	
	case WM_CHAR:
		switch (wParam)
		{
		case 'l':
		case 'L':
			if (bLight == false)
			{
				bLight = true;
				glEnable(GL_LIGHTING);
			}
			else
			{
				bLight = false;
				glDisable(GL_LIGHTING);
			}
			break;

			
		case 'X':
		case 'x':
			keyPressed = 1;
			LightPosition[0] = 0.0f;
			LightPosition[1] = 0.0f;
			LightPosition[2] = 0.0f;
			LightPosition[3] = 0.0f;
			
			angleOfXRotation = 0.0f;

			break;

		case 'Y':
		case 'y':
			keyPressed = 2;
			LightPosition[0] = 0.0f;
			LightPosition[1] = 0.0f;
			LightPosition[2] = 0.0f;
			LightPosition[3] = 0.0f;
			glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

			angleOfYRotation = 0.0f;

			break;

		case 'Z':
		case 'z':
			keyPressed = 3;
			LightPosition[0] = 0.0f;
			LightPosition[1] = 0.0f;
			LightPosition[2] = 0.0f;
			LightPosition[3] = 0.0f;
			glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

			angleOfZRotation = 0.0f;

			break;
			
		}
		break;

	case WM_DESTROY:
		uninitialize();
		PostQuitMessage(0);
		break;

	}
	
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

int initialize()
{
	void ReSize(int, int);
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;
	memset((void*)& pfd, NULL, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	ghdc = GetDC(ghwnd);

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);


	if (iPixelFormatIndex == 0)
	{
		return(-1);
	}
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		return(-2);
	}

	ghrc = wglCreateContext(ghdc);

	if (ghrc == NULL)
	{
		return(-3);
	}
	ghrc = wglCreateContext(ghdc);
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		return(-4);
	}
	glShadeModel(GL_SMOOTH);

	//Depth Test

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	//Configuration of Lights using Configuration Parameters
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	//We are not using Light Specular as we are going to use Material Specular

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, Light_Model_Local_Viewer);

	glEnable(GL_LIGHT0);  // This func has no value because by default Light is enabled as GL_LIGHTO

	for (int i = 0; i < 24; i++)
	{
		quadric[i] = gluNewQuadric();
	}
	
	//clear the screen by openGL color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	ReSize(WIN_WIDTH, WIN_HEIGHT);
	return(0);
}

void uninitialize(void)
{
	if (bFullScreen == true)
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
			HWND_TOP, 0, 0, 0, 0,
			SWP_NOZORDER | SWP_FRAMECHANGED |
			SWP_NOMOVE | SWP_NOSIZE |
			SWP_NOOWNERZORDER);
		ShowCursor(TRUE);
	}

	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;

	}
	if (gpFile)
	{
		fprintf(gpFile, "Log file is closed successfully\n");
		fclose(gpFile);
		gpFile = NULL;
	}
	for (int i = 0; i < 24; i++)
	{
		if (quadric)
		{
			gluDeleteQuadric(quadric[i]);
			quadric[i] = NULL;
		}
	}
}

void display(void)
{
	void Draw24Sphere(void);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (keyPressed == 1)
	{ 
		glRotatef(angleOfXRotation, 1.0f, 0.0f,0.0f);
		LightPosition[1] = angleOfXRotation;
	}
	else if(keyPressed == 2)
	{
		glRotatef(angleOfYRotation, 0.0f, 1.0f, 0.0f);
		LightPosition[2] = angleOfYRotation;
	}
	else if (keyPressed == 3)
	{
		glRotatef(angleOfZRotation, 0.0f, 0.0f, 1.0f);
		LightPosition[0] = angleOfZRotation;
	}
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	Draw24Sphere();

	SwapBuffers(ghdc);

}

void Draw24Sphere(void)
{

	GLfloat MaterialAmbient[4];
	GLfloat MaterialDiffuse[4];
	GLfloat MaterialSpecular[4];
	GLfloat MaterialShininess[1];
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	//Emerald

	MaterialAmbient[0] = 0.215f;
	MaterialAmbient[1] = 0.1745f;
	MaterialAmbient[2] = 0.0215f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.07568f; 
	MaterialDiffuse[1] = 0.61424f;
	MaterialDiffuse[2] = 0.07568f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.633f;
	MaterialSpecular[1] = 0.727811f;
	MaterialSpecular[2] = 0.633f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);
	
	MaterialShininess[0] = 0.6f * 128.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(6.0f, 14.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	
	//2. Jade
	MaterialAmbient[0] = 0.135f;
	MaterialAmbient[1] = 0.225f;
	MaterialAmbient[2] = 0.1575f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.54f;
	MaterialDiffuse[1] = 0.89f;
	MaterialDiffuse[2] = 0.63f;
	MaterialDiffuse[3] = 1.0f;
		
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , MaterialDiffuse);

	MaterialSpecular[0] = 0.316228f;
	MaterialSpecular[1] = 0.316228f;
	MaterialSpecular[2] = 0.316228f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.1f * 128.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(6.0f, 11.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//3. Obsidian
	MaterialAmbient[0] = 0.05375f;
	MaterialAmbient[1] = 0.5f;
	MaterialAmbient[2] = 0.06625f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.18275f;
	MaterialDiffuse[1] = 0.17f;
	MaterialDiffuse[2] = 0.22525f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.332741f;
	MaterialSpecular[1] = 0.328634f;
	MaterialSpecular[2] = 0.346434f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.3f * 128.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(6.0f, 9.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//4.Pearl
	MaterialAmbient[0] = 0.25f;
	MaterialAmbient[1] = 0.20725f;
	MaterialAmbient[2] = 0.20725f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 1.0f;
	MaterialDiffuse[1] = 0.829f;
	MaterialDiffuse[2] = 0.829f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.296648f;
	MaterialSpecular[1] = 0.296648f;
	MaterialSpecular[2] = 0.296648f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.088f * 128.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(6.0f, 6.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//5. Ruby
	MaterialAmbient[0] = 0.1745f;
	MaterialAmbient[1] = 0.01175f;
	MaterialAmbient[2] = 0.01175f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.6424f;
	MaterialDiffuse[1] = 0.04136f;
	MaterialDiffuse[2] = 0.04136f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.727811f;
	MaterialSpecular[1] = 0.626959f;
	MaterialSpecular[2] = 0.626959f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.6f * 128.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(6.0f, 4.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//6. Turquoise
	MaterialAmbient[0] = 0.1f;
	MaterialAmbient[1] = 0.18725f;
	MaterialAmbient[2] = 0.1745f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.396f;
	MaterialDiffuse[1] = 0.74151f;
	MaterialDiffuse[2] = 0.69102f;
	MaterialDiffuse[3] = 1.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.297254f;
	MaterialSpecular[1] = 0.30829f;
	MaterialSpecular[2] = 0.306678f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.1f * 128.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(6.0f, 1.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//2.1 Brass
	MaterialAmbient[0] = 0.329412f;
	MaterialAmbient[1] = 0.223529f;
	MaterialAmbient[2] = 0.027451f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.780392f;
	MaterialDiffuse[1] = 0.568627f;
	MaterialDiffuse[2] = 0.027451f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.992157f;
	MaterialSpecular[1] = 0.941176f;
	MaterialSpecular[2] = 0.807843f;
	MaterialSpecular[3] = 1.0f;

		
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.21794872f * 128.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(10.5f, 14.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);


	// 2.2 Bronze
	MaterialAmbient[0] = 0.2125f;
	MaterialAmbient[1] = 0.1275f;
	MaterialAmbient[2] = 0.054f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialAmbient);

	MaterialDiffuse[0] = 0.714f;
	MaterialDiffuse[1] = 0.4284f;
	MaterialDiffuse[2] = 0.18144f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialDiffuse);

	MaterialSpecular[0] = 0.393548f;
	MaterialSpecular[1] = 0.271906f;
	MaterialSpecular[2] = 0.166721f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.2f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(10.5f, 11.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//2.3 Chrome
	MaterialAmbient[0] = 0.25f;
	MaterialAmbient[1] = 0.25f;
	MaterialAmbient[2] = 0.25f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.4f;
	MaterialDiffuse[1] = 0.4f;
	MaterialDiffuse[2] = 0.4f;
	MaterialDiffuse[3] = 1.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.774597f;
	MaterialSpecular[1] = 0.774597f;
	MaterialSpecular[2] = 0.774597f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.6f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(10.5f, 9.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//2.4 Copper

	MaterialAmbient[0] = 0.19125f;
	MaterialAmbient[1] = 0.0735f;
	MaterialAmbient[2] = 0.0225f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.7038f;
	MaterialDiffuse[1] = 0.27048f;
	MaterialDiffuse[2] = 0.0828f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.256777f;
	MaterialSpecular[1] = 0.137622f;
	MaterialSpecular[2] = 0.086014f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.1f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(10.5f, 6.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	// 2.5 Gold

	MaterialAmbient[0] = 0.24725f;
	MaterialAmbient[1] = 0.1995f;
	MaterialAmbient[2] = 0.0745f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.75164f;
	MaterialDiffuse[1] = 0.60648f;
	MaterialDiffuse[2] = 0.22648f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.628281f;
	MaterialSpecular[1] = 0.555802f;
	MaterialSpecular[2] = 0.366065f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.4f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(10.5f, 4.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	
	//2.6 Silver

	MaterialAmbient[0] = 0.19225f;
	MaterialAmbient[1] = 0.19225f;
	MaterialAmbient[2] = 0.19225f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.50754f;
	MaterialDiffuse[1] = 0.50754f;
	MaterialDiffuse[2] = 0.50754f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.508273f;
	MaterialSpecular[1] = 0.508273f;
	MaterialSpecular[2] = 0.508273f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.4f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(10.5f, 1.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//3.1 Black
	MaterialAmbient[0] = 0.0f;
	MaterialAmbient[1] = 0.0f;
	MaterialAmbient[2] = 0.0f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.01f;
	MaterialDiffuse[1] = 0.01f;
	MaterialDiffuse[2] = 0.01f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.50f;
	MaterialSpecular[1] = 0.50f;
	MaterialSpecular[2] = 0.50f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.25f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(15.0f, 14.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	// 3.2 Cyan

	MaterialAmbient[0] = 0.0f;
	MaterialAmbient[1] = 0.1f;
	MaterialAmbient[2] = 0.06f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.0f;
	MaterialDiffuse[1] = 0.50980392f;
	MaterialDiffuse[2] = 0.50980392f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.50196078f;
	MaterialSpecular[1] = 0.50196078f;
	MaterialSpecular[2] = 0.50196078f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.25f * 128.0f;

		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(15.0f, 11.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//3.3 Green

	MaterialAmbient[0] =0.0f;
	MaterialAmbient[1] =0.0f;
	MaterialAmbient[2] =0.0f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.1f;
	MaterialDiffuse[1] = 0.35f;
	MaterialDiffuse[2] = 0.1f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.45f;
	MaterialSpecular[1] = 0.55f;
	MaterialSpecular[2] = 0.45f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.25f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(15.0f, 9.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//3.4 Red

	MaterialAmbient[0] = 0.0f;
	MaterialAmbient[1] = 0.0f;
	MaterialAmbient[2] = 0.0f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.5f;
	MaterialDiffuse[1] = 0.0f;
	MaterialDiffuse[2] = 0.0f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.7f;
	MaterialSpecular[1] = 0.6f;
	MaterialSpecular[2] = 0.6f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.25f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(15.0f, 6.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//3.5 White 

	MaterialAmbient[0] = 0.0f;
	MaterialAmbient[1] =0.0f;
	MaterialAmbient[2] =0.0f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.55f;
	MaterialDiffuse[1] = 0.55f;
	MaterialDiffuse[2] = 0.55f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] =0.70f;
	MaterialSpecular[1] =0.70f;
	MaterialSpecular[2] =0.70f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.25f * 128.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(15.0f, 4.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//3.6 Yellow
	MaterialAmbient[0] =0.0f;
	MaterialAmbient[1] =0.0f;
	MaterialAmbient[2] =0.0f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] =0.5f;
	MaterialDiffuse[1] =0.5f;
	MaterialDiffuse[2] = 0.0f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.60f;
	MaterialSpecular[1] = 0.60f;
	MaterialSpecular[2] = 0.50f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.25f * 128.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(15.0f, 1.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);

	//4.1 Black

	MaterialAmbient[0] = 0.02f;
	MaterialAmbient[1] = 0.02f;
	MaterialAmbient[2] = 0.02f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.01f;
	MaterialDiffuse[1] = 0.01f;
	MaterialDiffuse[2] = 0.01f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] =0.4f;
	MaterialSpecular[1] =0.4f;
	MaterialSpecular[2] =0.4f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.078125f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(19.5f, 14.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);




	//4.2 Cyan

	MaterialAmbient[0] = 0.0f;
	MaterialAmbient[1] = 0.05f;
	MaterialAmbient[2] = 0.05f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.4f;
	MaterialDiffuse[1] = 0.5f;
	MaterialDiffuse[2] = 0.5f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.04f;
	MaterialSpecular[1] = 0.7f;
	MaterialSpecular[2] = 0.7f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.078125f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(19.5f, 11.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);



	//4.3 Green

	MaterialAmbient[0] = 0.0f;
	MaterialAmbient[1] = 0.05f;
	MaterialAmbient[2] = 0.0f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.4f;
	MaterialDiffuse[1] = 0.5f;
	MaterialDiffuse[2] = 0.4f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.04f;
	MaterialSpecular[1] = 0.7f;
	MaterialSpecular[2] = 0.04f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.078125f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);


	glLoadIdentity();
	glTranslatef(19.5f, 9.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);


	//4.4 Red

	MaterialAmbient[0] = 0.5f;
	MaterialAmbient[1] = 0.0f;
	MaterialAmbient[2] = 0.0f;
	MaterialAmbient[3] = 1.0f;
		
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.5f;
	MaterialDiffuse[1] = 0.4f;
	MaterialDiffuse[2] = 0.4f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.7f;
	MaterialSpecular[1] = 0.04f;
	MaterialSpecular[2] = 0.04f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.078125 * 125;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(19.5f, 6.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);


	//4.5 White

	MaterialAmbient[0] = 0.05f;
	MaterialAmbient[1] = 0.05f;
	MaterialAmbient[2] = 0.05f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.5f;
	MaterialDiffuse[1] = 0.5f;
	MaterialDiffuse[2] = 0.5f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.7f;
	MaterialSpecular[1] = 0.7f;
	MaterialSpecular[2] = 0.7f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.078125f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(19.5f, 4.0f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);


	//4.6 Yellow

	MaterialAmbient[0] = 0.5f;
	MaterialAmbient[1] = 0.5f;
	MaterialAmbient[2] = 0.0f;
	MaterialAmbient[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	MaterialDiffuse[0] = 0.5f;
	MaterialDiffuse[1] = 0.5f;
	MaterialDiffuse[2] = 0.4f;
	MaterialDiffuse[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	MaterialSpecular[0] = 0.7f;
	MaterialSpecular[1] = 0.7f;
	MaterialSpecular[2] = 0.04f;
	MaterialSpecular[3] = 1.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);

	MaterialShininess[0] = 0.078125f * 128.0f;
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(19.5f, 1.5f, 0.0f);

	gluSphere(quadric[0], 1.0f, 30, 30);
	   
}


void ToggleFullScreen(void)
{
	MONITORINFO MI;
	if (bFullScreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			MI = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY),
				&MI))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP,
					MI.rcMonitor.left,
					MI.rcMonitor.top,
					MI.rcMonitor.right - MI.rcMonitor.left,
					MI.rcMonitor.bottom - MI.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
		bFullScreen = true;
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
			HWND_TOP, 0, 0, 0, 0,
			SWP_NOZORDER | SWP_FRAMECHANGED |
			SWP_NOMOVE | SWP_NOSIZE |
			SWP_NOOWNERZORDER);
		ShowCursor(TRUE);
		bFullScreen = false;
	}
}

void ReSize(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if (width < height)
	{
		glOrtho(0.0f, 15.5f, 0.0f, 15.5f *((GLfloat)height / (GLfloat)width), -10.0f, 10.0f);
	}
	else
	{
		glOrtho(0.0f, 15.5f*((GLfloat)width/(GLfloat)height), 0.0f, 15.5f, -10.0f, 10.0f);
	}
		
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void Update()
{
	if (keyPressed == 1)
	{
		angleOfXRotation = angleOfXRotation + 0.5f;
		if (angleOfXRotation > 360.0f)
		{
			angleOfXRotation = 0.0f;
		}
	}
	else if (keyPressed == 2)
	{
		angleOfYRotation = angleOfYRotation + 0.5f;
		if (angleOfYRotation > 360.0f)
		{
			angleOfYRotation = 0.0f;
		}
	}
	else if (keyPressed == 3)
	{
		angleOfZRotation = angleOfZRotation + 0.5f;
		if (angleOfZRotation > 360.0f)
		{
			angleOfZRotation = 0.0f;
		}
	}
	//angle_c = angle_c + 0.01f;
	//if (angle_c > 360.0f)
	//{
	//	angle_c = 0.0f;
	//}

}