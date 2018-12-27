#ifndef YASP_RENDER_CONTEXT_D3D_H
#define YASP_RENDER_CONTEXT_D3D_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/IRenderContext.h>
#include <d3d11.h>

namespace yasp
{
	class RenderContextD3D : public IRenderContext
	{
	public:
		RenderContextD3D(void* windowHandle);
		~RenderContextD3D();
	private:
		HWND windowHandle;
		ID3D11Device * device;
		ID3D11DeviceContext* deviceContext;
		IDXGISwapChain* swapChain;

		void CreateSwapChain();

	};
};

#endif
