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
	};

	struct BlendRenderTarget
	{
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
		BlendRenderTarget renderTargetBlend[8];
	};

	struct DepthStencilDesc
	{
		bool performTest;
		DepthWriteMask depthWriteMask;
		ComparisonFunc comparisonMode;
		bool writeStencil;
		uint8 stencilReadMask;
		uint8 stencilWriteMask;
		StencilOp onFrontFace;
		StencilOp onBackFace;
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
