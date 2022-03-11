#include "Window.h"

//Window* window = nullptr;

Window::Window()
{

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			// Event fired when the window will be created
			// collected here
			Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
			// .. and stored for later lookup
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
			window->setHWND(hwnd);
			window->OnCreate();
			break;
		}
		case WM_DESTROY:
		{
			// Event fired when the window will be destroyed
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->OnDestroy();
			::PostQuitMessage(0);
			break;
		}
		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

bool Window::init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc; // A procedure where we can handle different kinds of events !! Important property


	if (!::RegisterClassEx(&wc)) // If the registration of class will fail, the function will return false
	{
		return false;
	}

	/*if (!window)
	{
		window = this;
	}*/

	m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX App", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
		NULL, NULL, NULL, this);

	// if the creation fail return false
	if (!m_hwnd)
	{
		return false;
	}

	// Show the window
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	// set this flag to true to indicate that the window is initialized and running
	m_is_run = true;

	return true;
}

bool Window::broadcast()
{
	MSG msg;

	this->OnUpdate();
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// To avoid CPU throttle
	Sleep(1);

	return true;
}

bool Window::release()
{
	// Destroy window
	if (!::DestroyWindow(m_hwnd))
	{
		return false;
	}
	
	return true;
}

bool Window::isRun()
{
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}

void Window::OnCreate()
{
}

void Window::OnUpdate()
{
}

void Window::OnDestroy()
{
	m_is_run = false;
}

Window::~Window()
{
}
