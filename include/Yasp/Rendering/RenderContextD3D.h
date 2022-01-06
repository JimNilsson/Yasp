#ifndef YASP_RENDER_CONTEXT_D3D_H
#define YASP_RENDER_CONTEXT_D3D_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/IRenderContext.h>
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

		void SetTopology(Topology topology) override final;
		void SetViewport(Viewport viewport) override final;

		void Draw(uint32 vertexCount, uint32 offset) override final;
		void DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset) override final;
		void DrawInstanced(uint32 vertexCountPerInstance, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset) override final;
		void DrawAuto() override final;
		void DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 indexOffset, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset) override final;
	private:
		HWND windowHandle;
		ID3D11Device * device;
		ID3D11DeviceContext* deviceContext;
		ID3D11Debug* debug;
		IDXGISwapChain* swapChain;
		ID3D11DepthStencilView* backbufferDSV;
		ID3D11RenderTargetView* backbufferRTV;

		void CreateSwapChain();
		void CreateBackBuffer();

	};
};

#endif
