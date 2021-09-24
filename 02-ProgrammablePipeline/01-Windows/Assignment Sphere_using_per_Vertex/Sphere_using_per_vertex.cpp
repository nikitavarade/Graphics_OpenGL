
//#include<math.h>
#include"vmath.h"
#include"Sphere.h"
#include<stdio.h>
#include<windows.h>
#include<gl/GLEW.h>
#include<gl/GL.h>// this is for compiler
//#include<gl/GLU.h>// this is for compiler

#pragma comment(lib,"Sphere.lib")
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

GLuint gVao_sphere;
GLuint gVbo_sphere_position;
GLuint gVbo_sphere_element;
GLuint gVbo_sphere_normal;

//GLuint Uniform variables;

GLuint model_Matrix_Uniform;
GLuint View_Matrix_Uniform;
GLuint Projection_Matrix_Uniform;

GLuint la_Uniform;
GLuint ld_Uniform;
GLuint ls_Uniform;
GLuint Light_Position_Uniform;

GLuint ka_Uniform;
GLuint kd_Uniform;
GLuint ks_Uniform;
GLuint Material_Shininess_Uniform;

GLuint LKeyIsPressed;

mat4 perspectiveProjectionMatrix;

//Specify Light Configuration Parameters

GLfloat LightAmbient [4]= { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat LightDiffuse [4]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightSpecular[4] ={ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[4] = { 100.0f, 100.0f, 100.0f, 1.0f };

GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialSpecular[] ={ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialShininess =  50.0f ;

FILE* gpFile = NULL;
HWND ghwnd;
DWORD dwStyle;
bool bFullScreen = false;
bool gbLighting = false;
bool gbAnimation = false;
HDC ghdc = NULL;
HGLRC ghrc = NULL;// opengl rendering concept
bool gbActiveWindow = false;


GLfloat angle_Square = 0.0f;

//Sphere Variables
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
unsigned int gNumVertices;
unsigned int gNumElements;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function declaration
	void display(void);

	void Update(void);

	int initialize();
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Lights using per vertex on Sphere");
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
		TEXT("Lights using per vertex on Sphere"),
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
			if (gbActiveWindow == true && gbAnimation == true)
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

	case WM_CHAR:
		switch (wParam)
		{
		case 'L':
		case 'l':
			if (gbLighting == true)
			{
				gbLighting = false;
			}
			else
			{
				gbLighting = true;
			}
			break;
		case 'A':
		case 'a':
			if (gbAnimation == true)
			{
				gbAnimation = false;
			}
			else
			{
				gbAnimation = true;
			}
			break;
		}
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
		"in vec3 vNormal;"\

		"uniform vec3 u_la;"\
		"uniform vec3 u_ld;"\
		"uniform vec3 u_ls;"\
		"uniform vec4 u_Light_Position;"\

		"uniform vec3 u_ka;"\
		"uniform vec3 u_kd;"\
		"uniform vec3 u_ks;"\
		"uniform float u_Material_Shininess;"\

		"uniform mat4 u_Model_Matrix;" \
		"uniform mat4 u_View_Matrix;" \
		"uniform mat4 u_Projection_Matrix;"\

		"uniform int u_LKeyIsPressed;"\
		"out vec3 phong_ads_light;"\

		"void main(void)"\
		"{"\
			"if(u_LKeyIsPressed == 1)"\
			"{"\
				"vec4 eyeCoordinates = u_View_Matrix * u_Model_Matrix * vPosition;"\
				"vec3 TNorm = normalize(mat3(u_View_Matrix * u_Model_Matrix) * vNormal);"\
				"vec3 Light_Direction = normalize(vec3(u_Light_Position - eyeCoordinates));"\
				"float TN_dot_LD = max(dot(Light_Direction, TNorm),0.0);"\
				"vec3 Reflection_Vector = reflect(-Light_Direction, TNorm);"\
				"vec3 Viewer_Vector = normalize(vec3(-eyeCoordinates.xyz));"\
				"vec3 Ambient = u_la * u_ka ;"\
				"vec3 Diffuse = u_ld * u_kd * TN_dot_LD;"\
				"vec3 Specular = u_ls * u_ks * pow(max(dot(Reflection_Vector, Viewer_Vector),0.0),u_Material_Shininess);"\
				//"vec3 Specular = u_ls * u_ks * pow(1.0,u_Material_Shininess);"
				"phong_ads_light = Ambient + Diffuse + Specular;"\
			"}"\
			"else"\
			"{"\
				"phong_ads_light = vec3(1.0f,1.0f,1.0f);"\
			"}"\
			
			"gl_Position = u_Projection_Matrix * u_View_Matrix * u_Model_Matrix * vPosition; "\
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
		"in vec3 phong_ads_light;"\
		"uniform int u_LKeyIsPressed;"\
		"out vec4 FragColor;" \

		"void main(void)" \
		"{" \
				"if(u_LKeyIsPressed==1)"\
				"{"\
				      "FragColor = vec4(phong_ads_light, 1.0);"\
				"}"\
				"else"\
				"{"\
					   "FragColor = vec4(1.0,1.0,1.0,1.0);"\
				"}"\
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
		AMC_ATTRIBUTE_NORMAL,
		"vNormal");


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
	
	model_Matrix_Uniform = glGetUniformLocation(gShaderProgramObject, "u_Model_Matrix");
	View_Matrix_Uniform = glGetUniformLocation(gShaderProgramObject, "u_View_Matrix");
	Projection_Matrix_Uniform = glGetUniformLocation(gShaderProgramObject, "u_Projection_Matrix");

	LKeyIsPressed = glGetUniformLocation(gShaderProgramObject, "u_LKeyIsPressed");
	
	la_Uniform = glGetUniformLocation(gShaderProgramObject, "u_la");
	ld_Uniform = glGetUniformLocation(gShaderProgramObject, "u_ld");
	ls_Uniform = glGetUniformLocation(gShaderProgramObject, "u_ls");
	
	Light_Position_Uniform	= glGetUniformLocation(gShaderProgramObject, "u_Light_Position");

	ka_Uniform = glGetUniformLocation(gShaderProgramObject, "u_ka");
	kd_Uniform = glGetUniformLocation(gShaderProgramObject, "u_kd");
	ks_Uniform = glGetUniformLocation(gShaderProgramObject, "u_ks");
	
	Material_Shininess_Uniform = glGetUniformLocation(gShaderProgramObject, "u_Material_Shininess");

	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	//Sphere

	{

		glGenVertexArrays(1, &gVao_sphere);
		glBindVertexArray(gVao_sphere);

		//Position vbo
		{
			glGenBuffers(1, &gVbo_sphere_position);
			glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);
			glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		//Normal vbo
		{
			glGenBuffers(1, &gVbo_sphere_normal);
			glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
			glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

			glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		//Element vbo
		{
			glGenBuffers(1, &gVbo_sphere_element);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		//Unbind vao
		glBindVertexArray(0);
	}

	//Depth Test

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//clear the screen by openGL color
	glClearDepth(1.0f);

	//glEnable(GL_FRONT_AND_BACK);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	ReSize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
}

void uninitialize(void)
{
	if (gVbo_sphere_element)
	{
		glDeleteBuffers(1, &gVbo_sphere_element);

		gVbo_sphere_element = 0;
	}
	if (gVbo_sphere_normal)
	{
		glDeleteBuffers(1, &gVbo_sphere_normal);

		gVbo_sphere_normal = 0;
	}

	if (gVbo_sphere_position)
	{
		glDeleteBuffers(1, &gVbo_sphere_position);

		gVbo_sphere_position = 0;
	}

	if (gVao_sphere)
	{
		glDeleteBuffers(1, &gVao_sphere);

		gVao_sphere = 0;
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

	//====declaration of separate m v p

	mat4 modelMatrix;
	mat4 viewMatrix;

	//mat4  RotationMatrix;
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();


	//Sphere
	{

		modelMatrix = translate(0.0f, 0.0f, -3.0f);
		//modelMatrix = modelViewMatrix * scale(0.75f, 0.75f, 0.75f);
		

		if (gbLighting == true)
		{
			
			glUniform1i(LKeyIsPressed, 1);
			glUniform3fv(la_Uniform, 1, LightAmbient);	//Light Ambient Property
			glUniform3fv(ld_Uniform, 1, LightDiffuse);	//Light Diffuse Property
			glUniform3fv(ls_Uniform, 1, LightSpecular);	//Light Specular Property

			glUniform3fv(ka_Uniform, 1, MaterialAmbient);	//Material Ambient Property
			glUniform3fv(kd_Uniform, 1, MaterialDiffuse);	//Material Diffuse Property
			glUniform3fv(ks_Uniform, 1, MaterialSpecular);	//Material Specular	Property
			
			glUniform1f(Material_Shininess_Uniform, MaterialShininess);	//Material Shininess

			glUniform4fv(Light_Position_Uniform, 1, LightPosition);

		}
		else
		{
			glUniform1i(LKeyIsPressed, 0);
		}
		//Send necessary matrices to shaders in respective uniforms

		glUniformMatrix4fv(model_Matrix_Uniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(View_Matrix_Uniform, 1, GL_FALSE, viewMatrix);

		glUniformMatrix4fv(Projection_Matrix_Uniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		//Bind with vao(this will avoid many repetative binding)

		glBindVertexArray(gVao_sphere);

		//Draw the necessary scene

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
		glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

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


}
