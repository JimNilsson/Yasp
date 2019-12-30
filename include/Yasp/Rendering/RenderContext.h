#ifndef YASP_RENDER_CONTEXT_H
#define YASP_RENDER_CONTEXT_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/IRenderContext.h>
#include <Yasp/Rendering/IGPUResourceManager.h>
#include <Yasp/Rendering/Descriptions.h>


namespace yasp
{
	class RenderContext
	{
	private:
		IRenderContext* renderContext;
		IGPUResourceManager* gpuResourceManager;
	public:
		RenderContext(void* windowHandle);
		~RenderContext();
		
		IGPUResourceManager* ResourceManager() { return gpuResourceManager; }
		void Clear();
		void Display();

		void SetTopology(Topology topology);
		void SetViewport(Viewport viewport);

		void Draw(uint32 vertexCount, uint32 offset);
		void DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset);
		void DrawInstanced(uint32 vertexCountPerInstance, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset);
		void DrawAuto();
		void DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 indexOffset, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset);


	};
};

#endif
