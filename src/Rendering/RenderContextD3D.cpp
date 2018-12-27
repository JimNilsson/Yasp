#include <Yasp/Rendering/RenderContextD3D.h>
#include <assert.h>

#pragma comment(lib, "d3d11.lib")

yasp::RenderContextD3D::RenderContextD3D(void * windowHandle)
{
	this->windowHandle = (HWND)windowHandle;
	CreateSwapChain();
}

yasp::RenderContextD3D::~RenderContextD3D()
{
	deviceContext->Release();
	swapChain->Release();
	device->Release();
}

void yasp::RenderContextD3D::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = windowHandle;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.Windowed = TRUE;

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1
	};
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, levels, 2, D3D11_SDK_VERSION, &scd, &swapChain, &device, &featureLevel, &deviceContext);
	assert(SUCCEEDED(hr));
}
