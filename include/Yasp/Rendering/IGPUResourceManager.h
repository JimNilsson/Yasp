#ifndef YASP_I_GPU_RESOURCE_MANAGER_H
#define YASP_I_GPU_RESOURCE_MANAGER_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/GPUResourceID.h>
#include <Yasp/Rendering/Descriptions.h>
#include <Yasp/Rendering/GPUBuffer.h>
#include <Yasp/Rendering/AssignableMemory.h>
#include <Yasp/Rendering/GPUBufferVariable.h>
#include <Yasp/Rendering/Shader.h>
#include <tuple>
#include <string>

namespace yasp
{
	class IGPUResourceManager
	{
	public:
		IGPUResourceManager() {};
		virtual ~IGPUResourceManager() = 0 {};
		virtual GPUBuffer CreateBuffer(const BufferDesc& bufferDesc, void* initialData) = 0;
		virtual Shader CreateVertexShader(const void* shaderSourceCode, size_t shaderSourceSize) = 0;
		virtual Shader CreateVertexShader(const std::string& filename) = 0;
		virtual Shader CreatePixelShader(const std::string& filename) = 0;
		virtual Shader CreatePixelShader(const void* shaderSourceCode, size_t shaderSourceSize) = 0;
		virtual Shader GetShader(const GPUResourceID& id) = 0;
		virtual size_t GetBufferElementCount(const GPUResourceID& id) = 0;
		virtual size_t GetBufferSize(const GPUResourceID& id) const = 0;
		virtual const std::string& GetBufferElementName(const GPUResourceID& id, size_t offset) = 0;
		virtual AssignableMemory GetBufferSegment(const GPUResourceID& id, size_t offset) = 0;

		virtual GPUResourceID CreateRasterizer(RasterizerDesc rasterizerDesc) = 0;
		virtual GPUResourceID CreateTexture2D(const Texture2DDesc& textureDesc, void* data = nullptr) = 0;
		virtual GPUResourceID CreateTexture2DView(const Texture2DViewDesc& textureViewDesc, const GPUResourceID& texture) = 0;
		virtual GPUResourceID CreateSampler(const SamplerDesc& samplerDesc) = 0;
		virtual GPUResourceID CreateDepthStencilState(const DepthStencilDesc& depthStencilDesc) = 0;
		virtual GPUResourceID CreateBlendState(const BlendStateDesc& blendStateDesc) = 0;
		/*	
		virtual GPUResourceID CreateDepthStencil(DepthStencilViewDesc depthStencilDesc) = 0;
		virtual GPUResourceID CreateRenderTarget(RenderTargetDesc renderTargetDesc) = 0;*/

		virtual void UpdateBuffer(const GPUResourceID& id, void* data, uint32 size) = 0;
		virtual void StageBuffer(const GPUResourceID& id, void* data, size_t size, size_t offset = 0) = 0;
		virtual void UpdateBuffer(const GPUResourceID& id) = 0;
		virtual AssignableMemory GetBufferSegment(const GPUResourceID& id, const std::string& identifier) = 0;
		virtual GPUBufferVariable GetBufferVariable(const GPUResourceID& id, const std::string& identifier) = 0;
		virtual GPUBufferVariable GetBufferVariable(const GPUResourceID& id, const std::string& identifier, size_t offset) = 0;
		virtual std::tuple<size_t, size_t> GetBufferVariableOffsetSize(const GPUResourceID& id, const std::string& identifier, const std::string& varName) = 0;
		virtual void PushState() = 0;
		virtual void PopState() = 0;

		virtual void SetVertexBuffer(const GPUResourceID& id, uint32 stride, uint32 offset) = 0;
		virtual void SetIndexBuffer(const GPUResourceID& id, IndexFormat format, uint32 offset) = 0;
		virtual void SetShaderBuffers(ShaderType shader, GPUResourceID* buffers, uint32 startSlot, uint32 count) = 0;
		virtual void SetShaderTextureViews(ShaderType shader, GPUResourceID* textureViews, uint32 startSlot, uint32 count) = 0;
		virtual void SetShaderSamplers(ShaderType shader, GPUResourceID* samplers, uint32 startSlot, uint32 count) = 0;
		virtual void SetVertexShader(const GPUResourceID& id) = 0;
		virtual void SetGeometryShader(const GPUResourceID& id) = 0;
		virtual void SetPixelShader(const GPUResourceID& id) = 0;
		virtual void SetRasterizer(const GPUResourceID& id) = 0;
		virtual void SetDepthStencilState(const GPUResourceID& id, uint32_t reference = 0) = 0;
		virtual void SetBlendState(const GPUResourceID& id, const float* blendFactor, uint32 mask) = 0;
	};
};

#endif
