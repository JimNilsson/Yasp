#ifndef I_RENDER_CONTEXT_H
#define I_RENDER_CONTEXT_H

#include <Yasp/Config.h>

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

	};
};
#endif
