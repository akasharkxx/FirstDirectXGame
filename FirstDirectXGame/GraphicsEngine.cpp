#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "d3dcompiler.h"

GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	// This is list of all the available drives through which we find the best driver
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,		// Direct access to hardware drive most performant
		D3D_DRIVER_TYPE_WARP,			// draw are executed on the CPU
		D3D_DRIVER_TYPE_REFERENCE	    // Slow performance
	};
	
	UINT num_driver_types = ARRAYSIZE(driver_types);

	// Important argument to create device higher the level more the features
	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0 
	};
	// Array is used to allow more features in future
	UINT num_features_levels = ARRAYSIZE(feature_levels); 

	HRESULT res = 0;
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		// this will create a directX device from which
		// we will have access to all the resources to render graphics
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels, 
			num_features_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);

		if (SUCCEEDED(res))
		{
			break;
		}

		++driver_type_index;

	}

	if (FAILED(res))
	{
		// failed to select driver type
		return false;
	}

	m_imm_device_context = new DeviceContext(m_imm_context); 

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	// This factory will help us to create the swap chain
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	return true;
}

bool GraphicsEngine::release()
{
	if (m_vs)m_vs->Release();
	if (m_ps)m_ps->Release();

	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	m_imm_device_context->release();
	m_d3d_device->Release();

	return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

ConstantBuffer* GraphicsEngine::createConstantBuffer()
{
	return new ConstantBuffer();
}

VertexShader* GraphicsEngine::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = new VertexShader();

	if (!vs->init(shader_byte_code, byte_code_size))
	{
		vs->release();
		return nullptr;
	}

	return vs;
}

PixelShader* GraphicsEngine::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = new PixelShader();

	if (!ps->init(shader_byte_code, byte_code_size))
	{
		ps->release();
		return nullptr;
	}

	return ps;
}

bool GraphicsEngine::complieVertexShader(const wchar_t* filename, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(::D3DCompileFromFile(filename, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob))) 
	{
		if(error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

bool GraphicsEngine::compliePixelShader(const wchar_t* filename, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(::D3DCompileFromFile(filename, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

bool GraphicsEngine::releaseCompiledShader()
{
	if (m_blob) m_blob->Release();

	return true;
}

// This were basic shaders to run first time
//bool GraphicsEngine::createShaders()
//{
//	ID3DBlob* errblob = nullptr;
//	D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_psblob, &errblob);
//	m_d3d_device->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), nullptr, &m_ps);
//	return true;
//}
//
//bool GraphicsEngine::setShaders()
//{
//	m_imm_context->PSSetShader(m_ps, nullptr, 0);
//	return true;
//}

GraphicsEngine* GraphicsEngine::get()
{
	// Only created on the stack at initilization and not at runtime
	// will be available at a single address only during the runtime

	static GraphicsEngine engine;

	return &engine;
}
