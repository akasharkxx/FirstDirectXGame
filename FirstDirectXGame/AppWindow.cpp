#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

_declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

void AppWindow::UpdateQuadPosition()
{
	constant cc;
	cc.m_time = ::GetTickCount64();

	m_deltaPos += m_deltaTime * 0.5f;

	if (m_deltaPos > 1.0f)
	{
		m_deltaPos = 0;
	}

	Matrix4x4 temp;

	m_deltaScale += m_deltaTime / 0.55f;

	////cc.m_world.SetTranslation(Vector3D::Lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_deltaPos));
	//cc.m_world.SetScale(Vector3D::Lerp(Vector3D(0.5f, 0.5f, 0), Vector3D(1.5f, 1.5f, 0), (sin(m_deltaScale) + 1.0f) * 0.5f));

	//temp.SetTranslation(Vector3D::Lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_deltaPos));

	//cc.m_world *= temp;

	cc.m_world.SetScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));

	temp.SetIdentity();
	temp.SetRotationZ(0.0f);
	cc.m_world *= temp;

	temp.SetIdentity();
	temp.SetRotationY(m_rot_y);
	cc.m_world *= temp;

	temp.SetIdentity();
	temp.SetRotationX(m_rot_x);
	cc.m_world *= temp;

	//cc.m_world.SetTranslation(Vector3D(0.0f, 0.0f, 0.0f));
	cc.m_view.SetIdentity();
	cc.m_proj.SetOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	Window::onCreate();
	
	InputSystem::get()->AddListener(this);
	
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();
	
	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	// For Triangle strip 
	vertex vertex_list[] = 
	{
		//X - Y - Z
		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0),  Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,0), Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0), Vector3D(0,0.2f,0) },
		{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(0,1,1), Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,1,0), Vector3D(0,0.2f,0) }

	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};

	m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->load(index_list, size_index_list);

	//GraphicsEngine::get()->createShaders();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->complieVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	// release memory taken be the shader
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compliePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	// release memory taken be the shader
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	UpdateQuadPosition();

	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);
	

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vb->getSizeVertexList(), 0);
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);

	m_swap_chain->present(true);

	m_oldDeltaTime = m_newDeltaTime;
	m_newDeltaTime = ::GetTickCount64();

	m_deltaTime = (m_oldDeltaTime)?((m_newDeltaTime - m_oldDeltaTime) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_ib->release();
	m_cb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->AddListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->RemoveListener(this);
}

void AppWindow::OnKeyDown(int key)
{
	if (key == 'W')
	{
		m_rot_x += 0.707f * m_deltaTime;
	}
	else if (key == 'S')
	{
		m_rot_x -= 0.707f * m_deltaTime;
	}
	else if (key == 'A')
	{
		m_rot_y += 0.707f * m_deltaTime;
	}
	else if (key == 'D')
	{
		m_rot_y -= 0.707f * m_deltaTime;
	}
}

void AppWindow::OnKeyUp(int key)
{
}

void AppWindow::OnMouseMove(const Point& delta_mouse_pos)
{
	m_rot_x -= delta_mouse_pos.m_y * m_deltaTime;
	m_rot_y -= delta_mouse_pos.m_x * m_deltaTime;
}

void AppWindow::OnLeftMouseDown(const Point& delta_mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::OnLeftMouseUp(const Point& delta_mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::OnRightMouseDown(const Point& delta_mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::OnRightMouseUp(const Point& delta_mouse_pos)
{
	m_scale_cube = 1.0f;
}

AppWindow::~AppWindow()
{
}