//CPP
#include<stdio.h>
#include<windows.h>
#include<gl/GL.h>// this is for compiler
#include<gl/GLU.h>// this is for compiler

#pragma comment(lib,"glu32.lib")//this statement is for linker
#pragma comment(lib,"opengl32.lib")//this statement is for linker

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define CHECK_IMAGE_WIDTH 64
#define CHECK_IMAGE_HEIGHT 64

GLubyte CheckImage[CHECK_IMAGE_WIDTH][CHECK_IMAGE_HEIGHT][4];

GLuint TextImage;

GLfloat angle_t = 0.0f, angle_c = 0.0f;

WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
//global function declaration

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

FILE* gpFile = NULL;
HWND ghwnd;
DWORD dwStyle;
bool bFullScreen = false;
HDC ghdc = NULL;
HGLRC ghrc = NULL;// opengl rendering concept
bool gbActiveWindow = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function declaration
	void display(void);
	void UpdateT(void);
	void UpdateC(void);
	void Update(void);
	void Line(void);

	int initialize();
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("CheckerBoard");
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
		TEXT("CheckerBoard"),
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
				//Update();
			}
			//display();
			//UpdateT();
			//Line();
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
	void display(void);
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

	case WM_PAINT:// here we are adding new case WM_ERASEBKGND instead of WM_PAINT
		display();		   //display() is getting called in game loop 
		//return(0);//do not go to defWidowProc
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

	case WM_DESTROY:
		uninitialize();
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

int initialize()
{
	void LoadTexture(void);
	void ReSize(int, int);

	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;
	memset((void*)& pfd, NULL, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL ;

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

	//clear the screen by openGL color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	LoadTexture();
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
	if (TextImage)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &TextImage);
	}
}
void LoadTexture(void)
{
	void MakeCheckImage(void);

	//code
	MakeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextImage);
	glBindTexture(GL_TEXTURE_2D, TextImage);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECK_IMAGE_WIDTH, CHECK_IMAGE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, &CheckImage);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

}

void MakeCheckImage(void)
{
	int i, j, c;
	for (i = 0; i < CHECK_IMAGE_HEIGHT; i++)
	{
		for (j = 0; j < CHECK_IMAGE_WIDTH; j++)
		{
			c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;

			CheckImage[i][j][0] = (GLubyte)c;
			CheckImage[i][j][1] = (GLubyte)c;
			CheckImage[i][j][2] = (GLubyte)c;
			CheckImage[i][j][3] = (GLubyte)255;
		}
	}
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.6f);
	//glBindTexture(GL_TEXTURE_2D, Texture_Smiley);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, -1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.0f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.41421f, 1.0f, -1.41421f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.41421f, -1.0f, -1.41421f);


	glEnd();

	//SwapBuffers(ghdc);
	glFlush();
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

	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 30.0f);
}

void Update()
{

	angle_c = angle_c + 0.01f;
	if (angle_c > 360.0f)
	{
		angle_c = 0.0f;
	}

}