#include <Windows.h>
#include <ctime>

LRESULT CALLBACK WndProc(
	HWND hWnd, // handle to window instance
	UINT msg, // message id
	WPARAM wParam, // message information (single information)
	LPARAM lParam // additional informations (multiple informations)
);

int WINAPI WinMain(
	HINSTANCE hInstance, // handle to own application instance
	HINSTANCE hPrevInstance, // deprecated
	LPSTR szCmdLine, // command line
	int iCmdShow // in which state the application should be started (Normal, Minimised, Maximised)
)
{
	srand(time(nullptr));

	// 1. describe window class
	WNDCLASS wc = {};
	wc.hInstance = hInstance; // application handle
	wc.hbrBackground = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
	//wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND + 1); // background color with the default os background color
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW); // arrow cursor
	wc.hIcon = LoadIcon(nullptr, IDI_ERROR); // application icon
	wc.lpszClassName = TEXT("First Window");
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;

	// 2. register window class
	if (!RegisterClass(&wc)) return 10;

	// 3. calculating window size (optional)
	int width = GetSystemMetrics(SM_CXSCREEN); // client screen width
	int height = GetSystemMetrics(SM_CYSCREEN); // client screen height
	RECT r{ 100, 100, 640, 480 }; // left-top corner, right-bottom corner
	DWORD style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&r, style, false); // add border width, etc. to window rect

	// 4. create window instance
	HWND hWnd = CreateWindow(
		wc.lpszClassName, // window class name
		wc.lpszClassName, // window title
		style, // visual window style
		r.left, r.top, // window position (origin left-top corner)
		r.right - r.left, r.bottom - r.top, // window size
		nullptr, // handle to parent window
		nullptr, // handle to menu instance
		hInstance, // application handle
		nullptr // optional parameters
	);

	if (!hWnd) return 15;

	// 5. show window
	ShowWindow(hWnd, iCmdShow);
	SetFocus(hWnd);

	// 6. run window
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, UINT_MAX, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Sleep(10);
		}
	}

	return msg.wParam;
}

RECT r = { 100, 100, 150, 150 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		if (wParam == VK_LEFT)
		{
			r.left -= 1;
			r.right -= 1;
		}
		if (wParam == 'C') SetCursor(LoadCursor(nullptr, IDC_HAND));
		return 0;

	case WM_PAINT:
	{
		// old GDI (Graphic Device Interface) painting
		PAINTSTRUCT ps = {};
		BeginPaint(hWnd, &ps);
		FillRect(ps.hdc, &r, CreateSolidBrush(RGB(255, 255, 0)));
		EndPaint(hWnd, &ps);

		return 0;
	}

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
