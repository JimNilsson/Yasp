#include <Yasp/Rendering/RenderContext.h>

#ifdef YASP_RENDERER_D3D
#include <Yasp/Rendering/RenderContextD3D.h>
#endif

yasp::RenderContext::RenderContext(void * windowHandle)
{
#ifdef YASP_RENDERER_D3D
	renderContext = new RenderContextD3D(windowHandle);
#endif
}

yasp::RenderContext::~RenderContext()
{
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
