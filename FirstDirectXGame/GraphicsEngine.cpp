#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"

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
	ID3D11DeviceContext* m_imm_context;
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

GraphicsEngine* GraphicsEngine::get()
{
	// Only created on the stack at initilization and not at runtime
	// will be available at a single address only during the runtime

	static GraphicsEngine engine;

	return &engine;
}
