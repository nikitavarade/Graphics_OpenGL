#include<stdio.h>
#include<windows.h>
#define _USE_MATH_DEFINES 1
#include<math.h>
#include<gl/GL.h>// this is for compiler
#pragma comment(lib,"opengl32.lib")//this statement is for linker
#include<gl/GLU.h>// this is for compiler
#pragma comment(lib,"glu32.lib")//this statement is for linker
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
//global function declaration

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

FILE *gpFile = NULL;
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
	int initialize();
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Static Deathly Hallows Symbol");
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
		TEXT("Static Deathly Hallows Symbol"),
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
	memset((void*)&pfd, NULL, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
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

	//clear the screen by openGL color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

}

void display(void)
{


	GLfloat count = 0.0f, x1 = 0.0f, x2 = -1.0f, x3 = 1.0f, a = 0.0f, b = 0.0f, c = 0.0f, Radius = 0.0f, Area = 0.0f;
	GLfloat angle_t = 0.0f, angle = 0.0f, y1 = 1.0f, y2 = -1.0f, y3 = -1.0f, Ox = 0.0f, Oy = 0.0f, Perimeter = 0.0f, S = 0.0f, z = 0.0f;
	GLfloat ax, bx, cx, ay, by, cy, a1;

	ax = x2 - x1;
	ay = y2 - y1;
	bx = x3 - x1;
	by = y3 - y1;
	cx = x3 - x2;
	cy = y3 - y2;

	a = (GLfloat)sqrt((pow(ax, 2)) + (pow(ay, 2)));
	b = (GLfloat)sqrt((pow(bx, 2)) + (pow(by, 2)));
	c = (GLfloat)sqrt((pow(cx, 2)) + (pow(cy, 2)));


	Perimeter = a + b + c;

	S = Perimeter / 2; //Semiperimeter

	Ox = ((x1 * a) + (x2 * b) + (x3 * c)) / Perimeter;
	Oy = ((y1 * a) + (y2 * b) + (y3 * c)) / Perimeter;

	Area = (GLfloat)sqrt(S * (S - a) * (S - b) * (S - c));

	a1 = S / 2;
	Radius = Area / S;

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(angle_t, 0.0f, 1.0f, 0.0f);

	//176,224,230 powder blue
	//93,138,168 air force blue

	glBegin(GL_LINE_LOOP);

	//glColor3f(0.7f, 0.9f, 0.9f);//powder blue


	glColor3f(0.1f, 0.1f, 0.1f);//air force blue


	glVertex3d(0.8f, 0.0f, 0.0f);
	glVertex3d(0.5f, 0.2f, 0.0f);
	glVertex3d(0.2f, 0.2f, 0.0f);
	glVertex3d(-0.2f, 0.6f, 0.0f);
	glVertex3d(-0.3f, 0.6f, 0.0f);
	glVertex3d(-0.1f, 0.2f, 0.0f);
	glVertex3d(-0.5f, 0.2f, 0.0f);
	glVertex3d(-0.7f, 0.4f, 0.0f);
	glVertex3d(-0.6f, 0.0f, 0.0f);
	glVertex3d(-0.7f, -0.4f, 0.0f);
	glVertex3d(-0.5f, -0.2f, 0.0f);
	glVertex3d(-0.2f, -0.2f, 0.0f);
	glVertex3d(-0.3f, -0.6f, 0.0f);
	glVertex3d(-0.2f, -0.6f, 0.0f);
	glVertex3d(0.2f, -0.2f, 0.0f);
	glVertex3d(0.5f, -0.2f, 0.0f);
	glVertex3d(0.8f, 0.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);

	glColor3f(0.4f, 0.5f, 0.7f);//air force blue

	//for I
	glVertex3d(-0.3f, 0.1f, 0.0f);
	glVertex3d(-0.2f, 0.1f, 0.0f);

	glVertex3d(-0.3f, -0.1f, 0.0f);
	glVertex3d(-0.2f, -0.1f, 0.0f);

	glVertex3d(-0.25f, 0.1f, 0.0f);
	glVertex3d(-0.25f, -0.1f, 0.0f);

	//for A

	glVertex3d(-0.1f, -0.1f, 0.0f);
	glVertex3d(0.0f, 0.1f, 0.0f);
	
	glVertex3d(0.0f, 0.1f, 0.0f);
	glVertex3d(0.1f, -0.1f, 0.0f);
	
	glVertex3d(0.05f, 0.0f, 0.0f);
	glVertex3d(-0.05f, 0.0f, 0.0f);

	//for F

	glVertex3d(0.2f, -0.1f, 0.0f);
	glVertex3d(0.2f, 0.1f, 0.0f);

	glVertex3d(0.2f, 0.1f, 0.0f);
	glVertex3d(0.3f, 0.1f, 0.0f);

	glVertex3d(0.2f, 0.0f, 0.0f);
	glVertex3d(0.3f, 0.0f, 0.0f);


	glEnd();


	//use SwapBuffer(ghdc) for double buffering instead of glFush() 
	SwapBuffers(ghdc);

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

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}
