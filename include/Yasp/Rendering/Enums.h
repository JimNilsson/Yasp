#ifndef YASP_ENUMS_H
#define YASP_ENUMS_H

#include <Yasp/Config.h>

namespace yasp
{
	enum Usage : int32
	{
		GPU_READ_WRITE,
		GPU_READ,
		GPU_READ_CPU_WRITE,
		CPU_READ_WRITE
	};

	enum BufferType : int32
	{
		VERTEX_BUFFER,
		INDEX_BUFFER,
		SHADER_BUFFER,
		SHADER_TEXTURE,
		STREAM_OUT,
		RENDER_TARGET,
		DEPTH_STENCIL,
		COMPUTE_ACCESS
	};
	
	enum Topology : int32
	{
		LINE_LIST,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STRIP
	};

	enum TextureBinding : int32
	{
		SHADER_INPUT = 1 << 0,
		STREAM_OUT = 1 << 1,
		RENDER_TARGET = 1 << 2,
		DEPTH_BUFFER = 1 << 3
	};

	enum Texture2DFormat : int32
	{
		R32G32B32A32,
		R24G8,
		R8G8B8A8
	};

	enum TextureFilter : int32
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

	enum TextureWrapping : int32
	{
		CLAMP,
		REPEAT,
		MIRROR,
		BORDER
	};

	enum BlendOp : int32
	{
		ADD,
		SUBTRACT,
		MIN,
		MAX
	};

	enum BlendFactor : int32
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

	enum CullMode : int32
	{
		BACK,
		FRONT,
		NONE
	};

	enum WindingOrder : int32
	{
		CLOCKWISE,
		COUNTERCLOCKWISE
	};

	enum FillMode : int32
	{
		WIREFRAME,
		SOLID
	};


};


#endif
