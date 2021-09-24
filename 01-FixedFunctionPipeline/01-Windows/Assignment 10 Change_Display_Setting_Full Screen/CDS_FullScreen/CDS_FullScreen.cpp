#include<windows.h>

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	//handle, message, word parameter, long parameter
bool bFullScreen = false;
														//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//variable declarations
	WNDCLASSEX wndclass;
	TCHAR szClassName[] = TEXT("FullScreen by Using ChangeDisplaySetting");
	HWND hwnd;
	MSG msg;
	

	//code
	//initialization of WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	//register above created class to OS
	RegisterClassEx(&wndclass);

	//creating window of above declared and registered class
	hwnd = CreateWindow(szClassName,
		TEXT("FullScreen: Nikita Varade"),
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);

	//Shwing the Window on Screen which was created in Memory
	ShowWindow(hwnd, iCmdShow);

	UpdateWindow(hwnd);

	//Message Loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	DEVMODE dm;

	HDC hdc;
	RECT rc;
	TCHAR str[] = TEXT("FullScreen By Using ChangeDisplaySetting()");
	PAINTSTRUCT PS;

	
	switch(iMsg)
	{
		case WM_PAINT:
			GetClientRect(hwnd, &rc);
			hdc = BeginPaint(hwnd, &PS);
			hdc = GetDC(hwnd);
			SetBkColor(hdc, RGB(0, 0, 0)); //Background Color is setting as Black
			SetTextColor(hdc, RGB(255, 255, 0)); //Text Color is setting as Yellow
			DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hwnd, &PS);//ReleaseDC(hwnd, hdc);
		break;

		case WM_KEYDOWN:

			switch (wParam)
			{
					case 0x46:
						if (bFullScreen == false)
						{
							dm.dmSize = sizeof(DEVMODE);//dmSize member of DEVMODE must be initialized to the size, in bytes of DEVMODE structure
							EnumDisplaySettings(NULL, 0, &dm);
							ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
							//MessageBox(hwnd, TEXT("if condition"), TEXT("Fullscreen "), MB_OK);
							bFullScreen = true;
						}
						else
						{
							
							ChangeDisplaySettings(NULL, 0);
							MessageBox(hwnd, TEXT("else condition"), TEXT("Fullscreen "), MB_OK);
							bFullScreen = false;
						}
						break;
			}

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}



