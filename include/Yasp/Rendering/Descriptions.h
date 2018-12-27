#ifndef YASP_DESCRIPTIONS_H
#define YASP_DESCRIPTIONS_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/Enums.h>

namespace yasp
{
	struct InputDesc
	{

	};

	struct BufferDesc
	{
		int32 size;
		int32 byteStride;
		BufferType bind;
		Usage usage;
	};
};

#endif
