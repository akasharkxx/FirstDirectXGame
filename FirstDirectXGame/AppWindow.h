#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputSystem.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow();

	void UpdateQuadPosition();

	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	// Inherited via InputListener
	virtual void OnKeyDown(int key) override;
	virtual void OnKeyUp(int key) override;

	virtual void OnMouseMove(const Point& delta_mouse_pos) override;
	virtual void OnLeftMouseDown(const Point& delta_mouse_pos) override;
	virtual void OnLeftMouseUp(const Point& delta_mouse_pos) override;
	virtual void OnRightMouseDown(const Point& delta_mouse_pos) override;
	virtual void OnRightMouseUp(const Point& delta_mouse_pos) override;
private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;
	ConstantBuffer* m_cb;
	VertexShader* m_vs;
	PixelShader* m_ps;

private:
	float m_oldDeltaTime;
	float m_newDeltaTime;
	float m_deltaTime;

	float m_deltaPos;
	float m_deltaScale;

	float m_rot_x { 0.0f };
	float m_rot_y { 0.0f };
	float m_scale_cube = 1;
};

