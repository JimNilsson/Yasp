#include <Yasp/Rendering/Buffer.h>

#ifdef YASP_RENDERER_D3D
#include <Yasp/Rendering/BufferD3D.h>
#endif

Buffer::Buffer()
{
#ifdef YASP_RENDERER_D3D
	buffer = new BufferD3D()
#endif
}

Buffer::~Buffer()
{
}
