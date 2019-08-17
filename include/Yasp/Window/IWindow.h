#ifndef YASP_IWINDOW_H
#define YASP_IWINDOW_H

#include <Yasp/Config.h>
#include <Yasp/Math.h>
#include <string>

namespace yasp
{
	class YASP_API_EXPORT IWindow
	{
	private:

	public:
		IWindow() {};
		virtual ~IWindow() = 0 {};
		virtual void* GetWindowHandle() = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual bool IsOpen() const = 0;
		virtual void SetCursorVisibility(bool visible) = 0;
		virtual void SetWindowVisibility(bool visible) = 0;
		virtual void SetGrabCursor(bool grabbing) = 0;
		virtual int32_t GetWidth() const = 0;
		virtual int32_t GetHeight() const = 0;
		virtual float GetAspectRatio() const = 0;;
		virtual void PollEvents() = 0;
		virtual void Close() = 0;

		NOMOVORCOPY(IWindow);
	};

};


#endif
