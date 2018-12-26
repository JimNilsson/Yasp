#include <Yasp/Window/Window.h>
#include <Yasp/Window/IWindow.h>
#ifdef YASP_PLATFORM_WIN32
#include <Yasp/Window/WindowWin32.h>
#endif

yasp::Window::Window(int32 width, int32 height, bool fullscreen)
{
	#ifdef YASP_PLATFORM_WIN32
	window = new WindowWin32(width, height, fullscreen);
	#endif
}

yasp::Window::~Window()
{
	delete window;
}

void* yasp::Window::GetWindowHandle()
{
	return window->GetWindowHandle();
}

void yasp::Window::SetTitle(const std::string & title)
{
	window->SetTitle(title);
}

bool yasp::Window::IsOpen() const
{
	return window->IsOpen();
}

void yasp::Window::SetCursorVisibility(bool visible)
{
	window->SetCursorVisibility(visible);
}

void yasp::Window::SetWindowVisibility(bool visible)
{
	window->SetWindowVisibility(visible);
}

void yasp::Window::SetGrabCursor(bool grabbing)
{
	window->SetGrabCursor(grabbing);
}

int32_t yasp::Window::GetWidth() const
{
	return window->GetWidth();
}

int32_t yasp::Window::GetHeight() const
{
	return window->GetHeight();
}

float yasp::Window::GetAspectRatio() const
{
	return window->GetAspectRatio();
}

void yasp::Window::PollEvents()
{
	window->PollEvents();
}
