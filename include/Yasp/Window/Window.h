#ifndef YASP_WINDOW_H
#define YASP_WINDOW_H

#include <Yasp/Config.h>
#include <Yasp/Math.h>
#include <Yasp/Window/IWindow.h>
#include <string>

namespace yasp
{
	class Window
	{
	private:
		IWindow * window;
	public:
		Window(int32 width, int32 height, bool fullscreen = false);
		~Window();
		void* GetWindowHandle();
		void SetTitle(const std::string& title);
		bool IsOpen() const;
		void SetCursorVisibility(bool visible);
		void SetWindowVisibility(bool visible);
		void SetGrabCursor(bool grabbing);
		int32_t GetWidth() const;
		int32_t GetHeight() const;
		float GetAspectRatio() const;
		void PollEvents();

		NOMOVORCOPY(Window);
	};
};


#endif
