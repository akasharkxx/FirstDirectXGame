#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	// Initialize the window
	bool init();
	bool broadcast();
	// Release the window
	bool release();
	bool isRun();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	// EVENTS
	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDestroy();

	~Window();
protected:
	HWND m_hwnd;
	bool m_is_run;
};

