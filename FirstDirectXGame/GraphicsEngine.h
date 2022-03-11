#pragma once
#include <d3d11.h>

class SwapChain;

class GraphicsEngine
{
public:
	GraphicsEngine();
	// Initialize the Graphics Engine and DirectX 11 Device
	bool init();
	// Release all the resources loaded
	bool release();
	~GraphicsEngine();

public:
	SwapChain* createSwapChain();

public:
	static GraphicsEngine* get();

private:
	// Below are output variables while create directX device
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	ID3D11DeviceContext* m_imm_context;

private:
	// This handles directX low level with affecting directX runtime
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	
private:
	// We can access private members of Graphics Engine in Sawp chain class
	friend class SwapChain;
};

