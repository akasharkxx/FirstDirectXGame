#include "AppWindow.h"

struct vec3 
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
};

AppWindow::AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();
	
	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	// for triangle list
	//vertex list[] =
	//{
	//	// Triangle 1
	//	// X - Y - X
	//	{ -0.5f, -0.5f, 0.0f }, // POS1
	//	{ -0.5f, 0.5f, 0.0f }, // POS2
	//	{ 0.5f, 0.5f, 0.0f }, // POS3

	//	// Triangle 2 
	//	{ 0.5f, 0.5f, 0.0f }, // POS1
	//	{ 0.5f, -0.5f, 0.0f }, // POS2
	//	{ -0.5f, -0.5f, 0.0f }
	//};

	// For Triangle strip
	vertex list[] =
	{
		// Triangle 1
		// X - Y - X
		{ -0.5f, -0.5f, 0.0f }, // POS1
		{ -0.5f, 0.5f, 0.0f }, // POS2
		{ 0.5f, -0.5f, 0.0f }, // POS3
		{ 0.5f, 0.5f, 0.0f } // POS3
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	UINT size_shader = 0;
	GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);

	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	GraphicsEngine::get()->setShaders();

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);

	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_swap_chain->release();
	GraphicsEngine::get()->release();
}

AppWindow::~AppWindow()
{
}