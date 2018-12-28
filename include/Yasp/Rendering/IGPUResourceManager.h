#ifndef YASP_I_GPU_RESOURCE_MANAGER_H
#define YASP_I_GPU_RESOURCE_MANAGER_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/GPUResourceID.h>
#include <Yasp/Rendering/Descriptions.h>

namespace yasp
{
	class IGPUResourceManager
	{
	public:
		IGPUResourceManager() {};
		virtual ~IGPUResourceManager() = 0 {};
		virtual GPUResourceID CreateBuffer(BufferDesc bufferDesc, void* initialData) = 0;
	/*	virtual GPUResourceID CreateTexture(TextureDesc textureDesc) = 0;
		virtual GPUResourceID CreateTextureView(TextureViewDesc textureViewDesc, GPUResourceID texture) = 0;
		virtual GPUResourceID CreateShader(ShaderDesc shaderDesc) = 0;
		virtual GPUResourceID CreateRasterizer(RasterizerDesc rasterizerDesc) = 0;
		virtual GPUResourceID CreateBlendState(BlendStateDesc blendStateDesc) = 0;
		virtual GPUResourceID CreateDepthTest(DepthStencilDesc depthTestDesc) = 0;
		virtual GPUResourceID CreateDepthStencil(DepthStencilViewDesc depthStencilDesc) = 0;
		virtual GPUResourceID CreateRenderTarget(RenderTargetDesc renderTargetDesc) = 0;*/
	};
};

#endif
