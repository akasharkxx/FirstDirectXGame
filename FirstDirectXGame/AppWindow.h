#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"

class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

	// Inherited via Window
	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnDestroy() override;

private:
	SwapChain* m_swap_chain;
};

