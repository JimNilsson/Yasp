#include <Yasp/Rendering/RenderContext.h>

#ifdef YASP_RENDERER_D3D
#include <Yasp/Rendering/RenderContextD3D.h>
#include <Yasp/Rendering/GPUResourceManagerD3D.h>
#endif

yasp::RenderContext::RenderContext(void * windowHandle)
{
#ifdef YASP_RENDERER_D3D
	renderContext = new RenderContextD3D(windowHandle);
	gpuResourceManager = new GPUResourceManagerD3D((RenderContextD3D*)renderContext);
#endif
}

yasp::RenderContext::~RenderContext()
{
	delete gpuResourceManager;
	delete renderContext;
}

void yasp::RenderContext::Clear()
{
	renderContext->Clear();
}

void yasp::RenderContext::Display()
{
	renderContext->Display();
}

void yasp::RenderContext::SetTopology(Topology topology)
{
	renderContext->SetTopology(topology);
}

void yasp::RenderContext::SetViewport(Viewport viewport)
{
	renderContext->SetViewport(viewport);
}

void yasp::RenderContext::Draw(uint32 vertexCount, uint32 offset)
{
	renderContext->Draw(vertexCount, offset);
}

void yasp::RenderContext::DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset)
{
	renderContext->DrawIndexed(indexCount, indexOffset, vertexOffset);
}

void yasp::RenderContext::DrawInstanced(uint32 vertexCountPerInstance, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset)
{
	renderContext->DrawInstanced(vertexCountPerInstance, vertexOffset, instanceCount, instanceOffset);
}

void yasp::RenderContext::DrawAuto()
{
	renderContext->DrawAuto();
}

void yasp::RenderContext::DrawIndexedInstanced(uint32 indexCountPerInstance, uint32 indexOffset, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset)
{
	renderContext->DrawIndexedInstanced(indexCountPerInstance, indexOffset, vertexOffset, instanceCount, instanceOffset);
}
