#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;

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
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();
	VertexShader* createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShader* createPixelShader(const void* shader_byte_code, size_t byte_code_size);
public:
	bool complieVertexShader(const wchar_t* filename, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool compliePixelShader(const wchar_t* filename, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool releaseCompiledShader();

	//// Default simple shaders
	//bool createShaders();
	//bool setShaders();

public:
	static GraphicsEngine* get();

private:
	DeviceContext* m_imm_device_context;

private:
	// Below are output variables while create directX device
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;

private:
	// This handles directX low level with affecting directX runtime
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	ID3D11DeviceContext* m_imm_context;

private:
	ID3DBlob* m_blob = nullptr;

	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
	
private:
	// We can access private members of Graphics Engine in Sawp chain class
	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexShader;
	friend class PixelShader;
};

