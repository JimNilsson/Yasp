#ifndef YASP_GPU_RESOURCE_MANAGER_D3D_H
#define YASP_GPU_RESOURCE_MANAGER_D3D_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/IGPUResourceManager.h>
#include <Yasp/Rendering/RenderContextD3D.h>
#include <d3d11.h>
#include <d3d11shader.h>
#include <unordered_map>
#include <Yasp/Rendering/IBuffer.h>
#include <Yasp/Rendering/BufferD3D.h>
#include <Yasp/Rendering/GPUBuffer.h>
#include <Yasp/Rendering/ShaderD3D.h>

namespace yasp
{
	class GPUResourceManagerD3D : public IGPUResourceManager
	{
	public:
		GPUResourceManagerD3D(RenderContextD3D* renderContext);
		~GPUResourceManagerD3D();

		GPUBuffer CreateBuffer(BufferDesc bufferDesc, void* initialData) override final;
		Shader CreateVertexShader(const std::string& filename) override final;
		Shader CreatePixelShader(const std::string& filename) override final;
		GPUResourceID CreateRasterizer(RasterizerDesc rasterizerDesc) override final;
		GPUResourceID CreateTexture2D(const Texture2DDesc& textureDesc, void* data = nullptr) override final;
		GPUResourceID CreateTexture2DView(const Texture2DViewDesc& textureViewDesc, const GPUResourceID& texture) override final;
		GPUResourceID CreateSampler(const SamplerDesc& samplerDesc) override final;

		void UpdateBuffer(const GPUResourceID& id, void* data, uint32 size) override final;

		void SetVertexBuffer(const GPUResourceID& id, uint32 stride, uint32 offset) override final;
		void SetIndexBuffer(const GPUResourceID& id, IndexFormat format, uint32 offset) override final;
		void SetShaderBuffers(ShaderType shader, GPUResourceID* buffers, uint32 startSlot, uint32 count) override final;
		void SetShaderTextureViews(ShaderType shader, GPUResourceID* textureViews, uint32 startSlot, uint32 count) override final;
		void SetShaderSamplers(ShaderType shader, GPUResourceID* samplers, uint32 startSlot, uint32 count) override final;
		void SetVertexShader(const GPUResourceID& id) override final;
		void SetGeometryShader(const GPUResourceID& id) override final;
		void SetPixelShader(const GPUResourceID& id) override final;
		void SetRasterizer(const GPUResourceID& id) override final;
		void SetBlendState(const GPUResourceID& id, const float* blendFactor, uint32 mask) override final;


	private:
		void PixelShaderReflection(ID3D10Blob* shaderByteCode, ShaderD3D& shader);
		void VertexShaderReflection(ID3DBlob* shaderByteCode, ShaderD3D& shader);
		void RegisterResourceBindings(const D3D11_SHADER_DESC& desc, ID3D11ShaderReflection* reflection, ShaderD3D& shader);


	private:
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		struct GPUResourceD3D
		{
			ResourceType type;
			union
			{
				void* ptr;
				ID3D11Buffer* buffer;
				ID3D11VertexShader* vertexShader;
				ID3D11GeometryShader* geometryShader;
				ID3D11PixelShader* pixelShader;
				ID3D11ComputeShader* computeShader;
				ID3D11Texture2D* texture2D;
				ID3D11ShaderResourceView* shaderResourceView;
				ID3D11RenderTargetView* renderTargetView;
				ID3D11DepthStencilView* depthStencilView;
				ID3D11DepthStencilState* depthStencilState;
				ID3D11RasterizerState* rasterizerState;
				ID3D11BlendState* blendState;
				ID3D11InputLayout* inputLayout;
				ID3D11SamplerState* sampler;
			};
			
		};
		uint32 resourceCounter;
		std::unordered_map<GPUResourceID, GPUResourceD3D, GPUResourceID::Hasher> resourceMap;
	};
};

#endif
