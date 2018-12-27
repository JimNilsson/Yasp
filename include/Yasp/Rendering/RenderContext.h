#ifndef YASP_RENDER_CONTEXT_H
#define YASP_RENDER_CONTEXT_H

#include <Yasp/Config.h>
#include <Yasp/Rendering/IRenderContext.h>
#include <Yasp/Rendering/Descriptions.h>


namespace yasp
{
	class RenderContext
	{
	private:
		IRenderContext * renderContext;
	public:
		RenderContext(void* windowHandle);
		~RenderContext();

		void Clear();
		void Display();
	};
};

#endif
