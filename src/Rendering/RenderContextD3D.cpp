#include <Yasp/Rendering/RenderContextD3D.h>
#include <assert.h>

#pragma comment(lib, "d3d11.lib")

yasp::RenderContextD3D::RenderContextD3D(void * windowHandle)
{
	this->windowHandle = (HWND)windowHandle;
	CreateSwapChain();
	CreateBackBuffer();
}

yasp::RenderContextD3D::~RenderContextD3D()
{
	backbufferDSV->Release();
	backbufferRTV->Release();
	deviceContext->Release();
	swapChain->Release();
	device->Release();
}

void yasp::RenderContextD3D::Clear()
{
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->ClearRenderTargetView(backbufferRTV, clearColor);
	deviceContext->ClearDepthStencilView(backbufferDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void yasp::RenderContextD3D::Display()
{
	swapChain->Present(0, 0);
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

void yasp::RenderContextD3D::CreateBackBuffer()
{
	ID3D11Texture2D* backbuffer;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
	assert(SUCCEEDED(hr));

	D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};
	rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(backbuffer, &rtvd, &backbufferRTV);
	assert(SUCCEEDED(hr));
	backbuffer->Release();

	D3D11_TEXTURE2D_DESC bbdesc;
	backbuffer->GetDesc(&bbdesc);
	D3D11_TEXTURE2D_DESC dsvtd;
	dsvtd.Width = bbdesc.Width;
	dsvtd.Height = bbdesc.Height;
	dsvtd.ArraySize = 1;
	dsvtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsvtd.Format = DXGI_FORMAT_R24G8_TYPELESS;
	dsvtd.SampleDesc.Count = 1;
	dsvtd.SampleDesc.Quality = 0;
	dsvtd.MipLevels = 1;
	dsvtd.MiscFlags = 0;
	dsvtd.Usage = D3D11_USAGE_DEFAULT;
	dsvtd.CPUAccessFlags = 0;

	ID3D11Texture2D* depthTex;
	hr = device->CreateTexture2D(&dsvtd, nullptr, &depthTex);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	dsvd.Flags = 0;

	hr = device->CreateDepthStencilView(depthTex, &dsvd, &backbufferDSV);
	assert(SUCCEEDED(hr));
	depthTex->Release();
}
