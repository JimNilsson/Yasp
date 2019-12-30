#ifndef I_RENDER_CONTEXT_H
#define I_RENDER_CONTEXT_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/Descriptions.h>

namespace yasp
{
	class IRenderContext
	{
	private:

	public:
		IRenderContext() {}
		virtual ~IRenderContext() = 0 {}

		virtual void Clear() = 0;
		virtual void Display() = 0;

		virtual void SetTopology(Topology topology) = 0;
		virtual void SetViewport(Viewport viewport) = 0;

		virtual void Draw(uint32 vertexCount, uint32 offset) = 0;
		virtual void DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset) = 0;
		virtual void DrawInstanced(uint32 vertexCountPerInstance, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset) = 0;
		virtual void DrawAuto() = 0;
		virtual void DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 indexOffset, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset) = 0;
	};
};
#endif
