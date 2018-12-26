#ifndef YASP_WINDOW_WIN32_H
#define YASP_WINDOW_WIN32_H

#include <Yasp/Window/IWindow.h>
#include <Yasp/Window/Keyboard.h>
#include <Windows.h>
#include <unordered_map>

namespace yasp
{
	class WindowWin32 : public IWindow
	{
	private:
		HWND hwnd;
		HINSTANCE hinstance;
		WNDCLASSEX wc;

		uint32_t width;
		uint32_t height;
		bool fullscreen;

		bool cursorVisible;

		bool open;
		static LRESULT CALLBACK OnEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static std::unordered_map<uint32_t, Keyboard::Key> keyMapping;
		void Close();
	public:
		WindowWin32(uint32_t width, uint32_t height, bool fullscreen = false);
		~WindowWin32();


		void* GetWindowHandle() override final;
		void SetTitle(const std::string& title) override final;
		bool IsOpen() const override final;
		void SetCursorVisibility(bool visible) override final;
		void SetWindowVisibility(bool visible) override final;
		void SetGrabCursor(bool grabbing) override final;
		int32_t GetWidth() const override final;
		int32_t GetHeight() const override final;
		float GetAspectRatio() const override final;
		void PollEvents() override final;
	};
};

#endif
