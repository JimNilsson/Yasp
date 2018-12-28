#ifndef YASP_ENUMS_H
#define YASP_ENUMS_H

#include <Yasp/Config.h>

namespace yasp
{
	enum class InputFormat : int32
	{
		FLOAT32_4,
		FLOAT32_3,
		FLOAT32_2,
		FLOAT32_1,
		UINT32_4,
		UINT32_3,
		UINT32_2,
		UINT32_1,
		INT32_4,
		INT32_3,
		INT32_2,
		INT32_1,
		UINT16_4,
		UINT16_3,
		UINT16_2,
		UINT16_1,
		INT16_4,
		INT16_3,
		INT16_2,
		INT16_1,
		UINT8_4,
		UINT8_3,
		UINT8_2,
		UINT8_1,
		INT8_4,
		INT8_3,
		INT8_2,
		INT8_1,
	};

	enum class TextureFormat : int32
	{
		TYPELESS32_4,
		TYPELESS32_3,
		TYPELESS32_2,
		TYPELESS32_1,
		FLOAT32_4,
		FLOAT32_3,
		FLOAT32_2,
		FLOAT32_1,
		INT32_4,
		INT32_3,
		INT32_2,
		INT32_1,
		UINT32_4,
		UINT32_3,
		UINT32_2,
		UINT32_1,
		TYPELESS16_4,
		FLOAT16_4,
		UNORM16_4,
		SNORM16_4,
		UINT16_4,
		INT16_4,
		TYPELESS24_TYPELESS8,
		DEPTH24_STENCIL8
	};

	enum class TextureBind : int32
	{
		SHADER_VIEW,
		RENDER_TARGET,
		DEPTH_TARGET
	};

	enum class TextureDimension : int32
	{
		D1,
		D1_ARRAY,
		D2,
		D2_ARRAY,
		D3,
		D3_ARRAY
	};

	enum class Usage : int32
	{
		GPU_READ_WRITE,
		GPU_READ,
		GPU_READ_CPU_WRITE,
		CPU_READ_WRITE
	};

	enum BufferBinding : int32
	{
		BIND_VERTEX_BUFFER,
		BIND_INDEX_BUFFER,
		BIND_SHADER_BUFFER,
		BIND_SHADER_TEXTURE,
		BIND_STREAM_OUT,
		BIND_RENDER_TARGET,
		BIND_DEPTH_STENCIL,
		BIND_COMPUTE_ACCESS
	};
	
	enum class Topology : int32
	{
		LINE_LIST,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STRIP
	};

	enum class TextureBinding : int32
	{
		SHADER_INPUT = 1 << 0,
		STREAM_OUT = 1 << 1,
		RENDER_TARGET = 1 << 2,
		DEPTH_BUFFER = 1 << 3
	};

	enum class Texture2DFormat : int32
	{
		R32G32B32A32,
		R24G8,
		R8G8B8A8
	};

	enum class TextureFilter : int32
	{
		POINT,
		BILINEAR,
		TRILINEAR,
		ANISOTROPIC_1,
		ANISOTROPIC_2,
		ANISOTROPIC_4,
		ANISOTROPIC_8,
		ANISOTROPIC_16
	};

	enum class TextureWrapping : int32
	{
		CLAMP,
		REPEAT,
		MIRROR,
		BORDER
	};

	enum class BlendOp : int32
	{
		ADD,
		SUBTRACT,
		MIN,
		MAX
	};

	enum class BlendFactor : int32
	{
		ZERO,
		ONE,
		SRC_COLOR,
		SRC_ALPHA,
		INV_SRC_COLOR,
		INV_SRC_ALPHA,
		DEST_COLOR,
		DEST_ALPHA,
		INV_DEST_COLOR,
		INV_DEST_ALPHA
	};

	enum class CullMode : int32
	{
		BACK,
		FRONT,
		NONE
	};

	enum class WindingOrder : int32
	{
		CLOCKWISE,
		COUNTERCLOCKWISE
	};

	enum class FillMode : int32
	{
		WIREFRAME,
		SOLID
	};

	enum class StencilOp : int32
	{
		KEEP,
		ZERO,
		REPLACE,
		INCR_SAT,
		DECR_SAT,
		INVERT,
		INCR,
		DECR
	};

	enum class StencilComparisonMode : int32
	{
		NEVER,
		LESS,
		EQUAL,
		LESS_EQUAL,
		GREATER,
		NOT_EQUAL,
		GREATER_EQUAL,
		ALWAYS
	};

	enum class DepthStencilViewFlag : int32
	{
		READ_WRITE,
		READ_ONLY_DEPTH,
		READ_ONLY_STENCIL
	};

	enum class DepthWriteMask : int32
	{
		ZERO,
		ALL
	};


};


#endif
