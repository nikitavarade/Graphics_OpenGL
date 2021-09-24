#include<windows.h>

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	//handle, message, word parameter, long parameter

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//variable declarations
	WNDCLASSEX wndclass;
	TCHAR szClassName[] = TEXT("Using WM_CHAR");
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
		TEXT("USING WM_CHAR: Nikita Varade"),
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
	switch (iMsg)
	{
		case WM_CHAR:
			switch (wParam)
			{
				case 'f':
					MessageBox(hwnd, TEXT("This is f key"), TEXT("My Message : Nikita Varade"), MB_OK);
					break;

				case 'F':
					MessageBox(hwnd, TEXT("This is F Key"), TEXT("My Message : Nikita Varade"), MB_OK);
					break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
	
}
