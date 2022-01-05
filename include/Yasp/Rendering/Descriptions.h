#ifndef YASP_DESCRIPTIONS_H
#define YASP_DESCRIPTIONS_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/Enums.h>
#include <Yasp/SwizzleVec.h>

namespace yasp
{
		
	struct InputLayoutDesc
	{
		const InputFormat* inputs;
	};

	struct BufferDesc
	{
		int32 size;
		int32 byteStride;
		BufferBinding bind;
		Usage usage;
	};
	
	struct Texture2DDesc
	{
		int32 width;
		int32 height;
		int32 mipLevels;
		int32 arraySize;
		TextureFormat format;
		int32 sampleCount;
		int32 sampleQuality;
		Usage usage;
		TextureBind bind;
	};

	struct Texture2DViewDesc
	{
		TextureFormat format;
		int32 mipLevels;
		int32 mostDetailedMip;
	};

	struct SamplerDesc
	{
		TextureFilter filter;
		TextureWrapping wrapModeU;
		TextureWrapping wrapModeV;
		TextureWrapping wrapModeW;
		float mipLODBias;
		ComparisonFunc comparisonFunc;
		float4 borderColor;
		float minLOD;
		float maxLOD;
		int32 maxAnisotropy;
	};
	
	struct ShaderDesc
	{
		int32 dicks;
	};

	struct RasterizerDesc
	{
		FillMode fillMode = FillMode::SOLID;
		CullMode cullMode = CullMode::BACK;
		WindingOrder windingOrder = WindingOrder::CLOCKWISE;
		bool enableScissor = false;
		bool enableDepthClip = true;
	};

	struct BlendRenderTarget
	{
		bool enable = false;
		BlendFactor srcBlend = BlendFactor::ONE;
		BlendFactor destBlend = BlendFactor::ZERO;
		BlendFactor srcBlendAlpha = BlendFactor::ONE;
		BlendFactor destBlendAlpha = BlendFactor::ZERO;
		BlendOp blendOpColor = BlendOp::ADD;
		BlendOp blendOpAlpha = BlendOp::ADD;
	};

	struct BlendStateDesc
	{
		bool independentBlending = false;
		bool alphaToCoverage = false;
		BlendRenderTarget renderTargetBlend[8];
	};

	struct DepthStencilDesc
	{
		bool stencilTest = false;
		DepthWriteMask depthWriteMask = DepthWriteMask::ALL;
		ComparisonFunc comparisonMode = ComparisonFunc::LESS;
		bool depthTest = true;
		uint8 stencilReadMask = 0xff;
		uint8 stencilWriteMask = 0xff;
		StencilOp onFrontFaceFail = StencilOp::KEEP;
		StencilOp onFrontFaceDepthFail = StencilOp::KEEP;;
		StencilOp onFrontFacePass = StencilOp::KEEP;;
		ComparisonFunc frontFaceComparison = ComparisonFunc::ALWAYS;;
		StencilOp onBackFaceFail = StencilOp::KEEP;;
		StencilOp onBackFaceDepthFail = StencilOp::KEEP;;
		StencilOp onBackFacePass = StencilOp::KEEP;;
		ComparisonFunc backFaceComparison = ComparisonFunc::ALWAYS;
	};

	struct Viewport
	{
		float topLeftX;
		float topLeftY;
		float width;
		float height;
		float minDepth;
		float maxDepth;
	};

	struct DepthStencilViewDesc
	{
		Texture2DFormat format;
		TextureDimension dimension;
		DepthStencilViewFlag flag;
		int32 mipSlice;
	};

	struct RenderTargetDesc
	{
		Texture2DFormat format;
		TextureDimension dimension;
		int32 mipSlice;
		int32 arraySlice;
		int32 arraySize;
	};

};

#endif
