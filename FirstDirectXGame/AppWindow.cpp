#include "AppWindow.h"

AppWindow::AppWindow()
{
}

void AppWindow::OnCreate()
{
	Window::OnCreate();
	GraphicsEngine::get()->init();
	m_swap_chain =  GraphicsEngine::get()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
}

void AppWindow::OnUpdate()
{
	Window::OnUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 1, 1, 0, 1);

	m_swap_chain->present(false);
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	m_swap_chain->release();
	GraphicsEngine::get()->release();
}

AppWindow::~AppWindow()
{
}