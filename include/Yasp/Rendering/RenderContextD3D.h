#ifndef YASP_RENDER_CONTEXT_D3D_H
#define YASP_RENDER_CONTEXT_D3D_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/IRenderContext.h>
#include <Yasp/Rendering/BufferD3D.h>
#include <Yasp/Rendering/Descriptions.h>
#include <d3d11.h>

namespace yasp
{
	class RenderContextD3D : public IRenderContext
	{
	public:
		RenderContextD3D(void* windowHandle);
		~RenderContextD3D();

		ID3D11Device* Device() { return device; }
		ID3D11DeviceContext* DeviceContext() { return deviceContext; }

		void Clear() override final;
		void Display() override final;
	private:
		HWND windowHandle;
		ID3D11Device * device;
		ID3D11DeviceContext* deviceContext;
		IDXGISwapChain* swapChain;
		ID3D11DepthStencilView* backbufferDSV;
		ID3D11RenderTargetView* backbufferRTV;

		void CreateSwapChain();
		void CreateBackBuffer();

	};
};

#endif
