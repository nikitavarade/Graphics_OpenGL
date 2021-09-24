
//#include<math.h>
#include "vmath.h"
#include<stdio.h>
#include<windows.h>
#include<gl/GLEW.h>
#include<gl/GL.h>// this is for compiler
//#include<gl/GLU.h>// this is for compiler

#pragma comment(lib,"glew32.lib")//this statement is for linker
//#pragma comment(lib,"C:\glew-2.1.0\lib\Release\Win32\glew32.lib")//this statement is for linker
#pragma comment(lib,"opengl32.lib")//this statement is for linker

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

using namespace vmath;
//using namespace vmath;

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXCOORD0
};

WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
//global function declaration

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Add Shader Global Variables
GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;
GLuint vao_Triangle;
GLuint vao_Square;
GLuint vbo_Position_Triangle;
GLuint vbo_Color_Triangle;
GLuint vbo_Position_Square;
GLuint vbo_Color_Square;
GLuint mvpUniform;
mat4 perspectiveProjectionMatrix;


FILE* gpFile = NULL;
HWND ghwnd;
DWORD dwStyle;
bool bFullScreen = false;
HDC ghdc = NULL;
HGLRC ghrc = NULL;// opengl rendering concept
bool gbActiveWindow = false;

GLfloat angle_Triangle = 0.0f;
GLfloat angle_Square = 0.0f;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function declaration
	void display(void);

	void Update(void);

	int initialize();
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("3D_Animation");
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
		TEXT("3D_Animation"),
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
				Update();
			}
			display();
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
	GLenum result;
	void ReSize(int, int);
	void uninitialize(void);


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
	result = glewInit();
	if (result != GLEW_OK)
	{
		fprintf(gpFile, "GLEW INIT Failed\n");
		uninitialize();
		DestroyWindow(ghwnd);
	}

	//Define Vertex Shader Object
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//Write Vertex Shader Code

	const GLchar* vertexShaderSourceCode =
		"#version 440 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec4 vColor;"

		"uniform mat4 u_mvp_matrix;" \
		"out vec4 out_Color;"

		"void main(void)" \
		"{" \
		"gl_Position = u_mvp_matrix * vPosition;" \
		"out_Color = vColor;"
		"}";



	//Specify above source code to the vertex Shader object

	glShaderSource(gVertexShaderObject, 1, (const GLchar * *)& vertexShaderSourceCode, NULL);

	//Compile the vertex Shader

	glCompileShader(gVertexShaderObject);

	//Steps for Catching error(Compile Status log)

	GLint iShaderCompileStatus = 0;
	GLint iInfoLoglength = 0;
	GLchar* szInfoLog = NULL;

	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
	if (iShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLoglength);
		if (iInfoLoglength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfoLoglength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject, iInfoLoglength, &written, szInfoLog);
				fprintf(gpFile, "\nVertex Shader Compilation Log - %s", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}

		}
	}

	//Fragment Shader Code

	//Define Fragment Shader Object
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//Write Fragment Shader Code

	const GLchar* fragmentShaderSourceCode =
		"#version 440 core" \
		"\n" \
		"in vec4 out_Color;"
		"out vec4 FragColor;" \


		"void main(void)" \
		"{" \
		"FragColor = out_Color;" \
		"}";

	//Specify above source code to the Fragment Shader object

	glShaderSource(gFragmentShaderObject, 1, (const GLchar * *)& fragmentShaderSourceCode, NULL);

	//Compile the Fragment Shader

	glCompileShader(gFragmentShaderObject);

	//Steps for Catching error(Compile Status log)

	iShaderCompileStatus = 0;
	iInfoLoglength = 0;
	szInfoLog = NULL;

	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
	if (iShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLoglength);
		if (iInfoLoglength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfoLoglength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject, iInfoLoglength, &written, szInfoLog);
				fprintf(gpFile, "\nFragment Shader Compilation Log - %s ", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}

		}
	}

	//Create Shader Program Object

	gShaderProgramObject = glCreateProgram();

	//Attach vertex shader to the shader program

	glAttachShader(gShaderProgramObject, gVertexShaderObject);

	//Attach Fragment shader to the shader program

	glAttachShader(gShaderProgramObject, gFragmentShaderObject);

	//Prelinking binding to vertex Attributes

	glBindAttribLocation(gShaderProgramObject,
		AMC_ATTRIBUTE_POSITION,
		"vPosition");

	glBindAttribLocation(gShaderProgramObject,
		AMC_ATTRIBUTE_COLOR,
		"vColor");


	//Link the Shader program

	glLinkProgram(gShaderProgramObject);

	//Linker Status Code

	GLint iProgramLinkStatus = 0;
	iInfoLoglength = 0;
	szInfoLog = NULL;

	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iProgramLinkStatus);
	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLoglength);
		if (iInfoLoglength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfoLoglength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLoglength, &written, szInfoLog);
				fprintf(gpFile, "\nProgram Link Log - %s", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}

		}
	}

	mvpUniform = glGetUniformLocation(gShaderProgramObject, "u_mvp_matrix");

	//triangle
	{
	
		const GLfloat triangleVertices[] = {
												0.0f ,  1.0f,  0.0f,
												-1.0f, -1.0f,  1.0f,
												1.0f , -1.0f,  1.0f,
												0.0f ,  1.0f,  0.0f,
												1.0f , -1.0f,  1.0f,
												1.0f , -1.0f, -1.0f,
												0.0f ,  1.0f,  0.0f,
												1.0f , -1.0f, -1.0f,
												-1.0f, -1.0f, -1.0f,
												0.0f ,  1.0f,  0.0f,
												-1.0f, -1.0f, -1.0f,
												-1.0f, -1.0f,  1.0f

		};

		const GLfloat triangleColor[] = {
											

								1.0f, 0.0f, 0.0f,  //R
								0.0f, 1.0f, 0.0f,  //G
								0.0f, 0.0f, 1.0f,  //B
								1.0f, 0.0f, 0.0f,  //R
								0.0f, 0.0f, 1.0f,  //B
								0.0f, 1.0f, 0.0f,  //G
								1.0f, 0.0f, 0.0f,  //R
								0.0f, 1.0f, 0.0f,  //G
								0.0f, 0.0f, 1.0f,  //B
								1.0f, 0.0f, 0.0f,  //R
								0.0f, 0.0f, 1.0f,  //B
								0.0f, 1.0f, 0.0f   //G
		};


		glGenVertexArrays(1, &vao_Triangle);

		glBindVertexArray(vao_Triangle);
		//Position vbo
		{
			glGenBuffers(1, &vbo_Position_Triangle);

			glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Triangle);

			glBufferData(GL_ARRAY_BUFFER,
				sizeof(triangleVertices),
				triangleVertices,
				GL_STATIC_DRAW);
			glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				NULL);

			glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		//Color vbo
		{
			glGenBuffers(1, &vbo_Color_Triangle);

			glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Triangle);

			glBufferData(GL_ARRAY_BUFFER,
				sizeof(triangleColor),
				triangleColor,
				GL_STATIC_DRAW);
			glVertexAttribPointer(AMC_ATTRIBUTE_COLOR,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				NULL);

			glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);
	}

	//Square
	{

		 const GLfloat SquareVertices[] = {
								
					  1.0f,  1.0f,  1.0f,
					 -1.0f,  1.0f,  1.0f,
					 -1.0f, -1.0f,  1.0f,
					  1.0f, -1.0f,  1.0f,
					  1.0f,  1.0f,  1.0f,
					  1.0f, -1.0f,  1.0f,
					  1.0f, -1.0f, -1.0f,
					  1.0f,  1.0f, -1.0f,
					  1.0f,  1.0f, -1.0f,
					  1.0f, -1.0f, -1.0f,
					 -1.0f, -1.0f, -1.0f,
					 -1.0f,  1.0f, -1.0f,
					 -1.0f, -1.0f, -1.0f,
					 -1.0f,  1.0f, -1.0f,
					 -1.0f,  1.0f,  1.0f,
					 -1.0f, -1.0f,  1.0f,
					 -1.0f,  1.0f,  1.0f,
					  1.0f,  1.0f,  1.0f,
					  1.0f,  1.0f, -1.0f,
					 -1.0f,  1.0f, -1.0f,
					 -1.0f, -1.0f,  1.0f,
					  1.0f, -1.0f,  1.0f,
					  1.0f, -1.0f, -1.0f,
					 -1.0f, -1.0f, -1.0f

		};

		const GLfloat SquareColor[] = {
											1.0f, 0.0f, 0.0f,//R 
											1.0f, 0.0f, 0.0f,//R 
											1.0f, 0.0f, 0.0f,//R 
											1.0f, 0.0f, 0.0f,//R 
											0.0f, 1.0f, 0.0f,//B 
											0.0f, 1.0f, 0.0f,//B 
											0.0f, 1.0f, 0.0f,//B 
											0.0f, 1.0f, 0.0f,//B 
											0.0f, 0.0f, 1.0f,//G
											0.0f, 0.0f, 1.0f,//G
											0.0f, 0.0f, 1.0f,//G
											0.0f, 0.0f, 1.0f,//G
											1.0f, 1.0f, 0.0f,//Y
											1.0f, 1.0f, 0.0f,//Y
											1.0f, 1.0f, 0.0f,//Y
											1.0f, 1.0f, 0.0f,//Y
											0.0f, 1.0f, 1.0f,//C
											0.0f, 1.0f, 1.0f,//C
											0.0f, 1.0f, 1.0f,//C
											0.0f, 1.0f, 1.0f,//C
											1.0f, 0.0f, 1.0f,//M
											1.0f, 0.0f, 1.0f,//M
											1.0f, 0.0f, 1.0f,//M
											1.0f, 0.0f, 1.0f //M

		};


		glGenVertexArrays(1, &vao_Square);

		glBindVertexArray(vao_Square);

		//Position vbo
		{
			glGenBuffers(1, &vbo_Position_Square);

			glBindBuffer(GL_ARRAY_BUFFER, vbo_Position_Square);

			glBufferData(GL_ARRAY_BUFFER,
				sizeof(SquareVertices),
				SquareVertices,
				GL_STATIC_DRAW);
			glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				NULL);

			glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		//color vbo
		{
			glGenBuffers(1, &vbo_Color_Square);

			glBindBuffer(GL_ARRAY_BUFFER, vbo_Color_Square);

			glBufferData(GL_ARRAY_BUFFER,
				sizeof(SquareColor),
				SquareColor,
				GL_STATIC_DRAW);
			glVertexAttribPointer(AMC_ATTRIBUTE_COLOR,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				NULL);

			glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);
	}
	glShadeModel(GL_SMOOTH);

	//Depth Test

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//clear the screen by openGL color
	glClearDepth(1.0f);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glEnable(GL_FRONT_AND_BACK);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	ReSize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
}

void uninitialize(void)
{
	if (vbo_Position_Triangle)
	{
		glDeleteBuffers(1, &vbo_Position_Triangle);
		vbo_Position_Triangle = 0;
	}

	if (vbo_Color_Triangle)
	{
		glDeleteBuffers(1, &vbo_Color_Triangle);
		vbo_Color_Triangle = 0;
	}

	if (vao_Triangle)
	{
		glDeleteBuffers(1, &vao_Triangle);
		vao_Triangle = 0;
	}

	if (vbo_Position_Square)
	{
		glDeleteBuffers(1, &vbo_Position_Square);

		vbo_Position_Square = 0;
	}

	if (vbo_Color_Square)
	{
		glDeleteBuffers(1, &vbo_Color_Square);

		vbo_Color_Square = 0;
	}

	if (vao_Square)
	{
		glDeleteBuffers(1, &vao_Square);

		vao_Square = 0;
	}


	glUseProgram(gShaderProgramObject);
	glDetachShader(gShaderProgramObject, gFragmentShaderObject);
	glDetachShader(gShaderProgramObject, gVertexShaderObject);

	glDeleteShader(gFragmentShaderObject);
	gFragmentShaderObject = 0;

	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;

	glDeleteProgram(gShaderProgramObject);
	gShaderProgramObject = 0;
	glUseProgram(0);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(gShaderProgramObject);

	//Declaration of matrices

	mat4 modelViewMatrix;

	mat4 modelViewProjectionMatrix;

	mat4  RotationMatrix;

	// Pyramid
	{
		//initialize above matrix to identity

		modelViewMatrix = mat4::identity();

		modelViewProjectionMatrix = mat4::identity();

		RotationMatrix = mat4::identity();

		//Do necessary matrix multiplication
		modelViewMatrix = translate(-1.5f, 0.0f, -6.0f);

		RotationMatrix = rotate(angle_Triangle, 0.0f, 1.0f, 0.0f);

		modelViewMatrix = modelViewMatrix * RotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;



		//Send necessary matrices to shaders in respective uniforms

		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		//Bind with vao(this will avoid many repetative binding)

		glBindVertexArray(vao_Triangle);

		//Draw the necessary scene

		glDrawArrays(GL_TRIANGLES, 0, 12);

		//unbind vao

		glBindVertexArray(0);
	}
	//Rectangle
	{

		modelViewMatrix = translate(1.5f, 0.0f, -6.0f);
		modelViewMatrix = modelViewMatrix* scale(0.75f, 0.75f, 0.75f);
		RotationMatrix = rotate(angle_Square,angle_Square,angle_Square);
		modelViewMatrix = modelViewMatrix * RotationMatrix;

		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;


		//Send necessary matrices to shaders in respective uniforms

		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, modelViewProjectionMatrix);

		//Bind with vao(this will avoid many repetative binding)

		glBindVertexArray(vao_Square);

		//Draw the necessary scene

		glDrawArrays(GL_TRIANGLE_FAN, 0,  4);
		glDrawArrays(GL_TRIANGLE_FAN, 4,  4);
		glDrawArrays(GL_TRIANGLE_FAN, 8,  4);
		glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 20, 4);


		//unbind vao

		glBindVertexArray(0);
	}
	glUseProgram(0);

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

	perspectiveProjectionMatrix = perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);


}
void Update(void)
{
	angle_Square = angle_Square + 0.05f;
	if (angle_Square > 360.0f)
	{
		angle_Square = 0.0f;
	}

	angle_Triangle = angle_Triangle + 0.05f;
	if (angle_Triangle > 360.0f)
	{
		angle_Triangle = 0.0f;
	}

}
