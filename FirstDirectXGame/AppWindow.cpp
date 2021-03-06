#include "AppWindow.h"

struct vec3 
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 color;
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
		// X - Y - X -- color
		{ -0.5f, -0.5f, 0.0f , 1,0,0}, // POS1
		{ -0.5f, 0.5f, 0.0f  , 0,1,0}, // POS2
		{ 0.5f, -0.5f, 0.0f  , 1,0,1}, // POS3
		{ 0.5f, 0.5f, 0.0f   , 1,1,0} // POS3
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	//GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->complieVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	// release memory taken be the shader
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compliePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	// release memory taken be the shader
	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	//GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);
	

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
	m_ps->release();
	GraphicsEngine::get()->release();
}

AppWindow::~AppWindow()
{
}