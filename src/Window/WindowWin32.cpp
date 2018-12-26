#include <Yasp/Config.h>
#ifdef YASP_PLATFORM_WIN32
#include <Yasp/Window/WindowWin32.h>
#include <Yasp/Window/Keyboard.h>
#include <Yasp/Window/Mouse.h>
#include <iostream>

std::unordered_map<uint32_t, yasp::Keyboard::Key> yasp::WindowWin32::keyMapping = {
	{ 0x41, yasp::Keyboard::Key::A },
	{ 0x42, yasp::Keyboard::Key::B },
	{ 0x43, yasp::Keyboard::Key::C },
	{ 0x44, yasp::Keyboard::Key::D },
	{ 0x45, yasp::Keyboard::Key::E },
	{ 0x46, yasp::Keyboard::Key::F },
	{ 0x47, yasp::Keyboard::Key::G },
	{ 0x48, yasp::Keyboard::Key::H },
	{ 0x49, yasp::Keyboard::Key::I },
	{ 0x4A, yasp::Keyboard::Key::J },
	{ 0x4B, yasp::Keyboard::Key::K },
	{ 0x4C, yasp::Keyboard::Key::L },
	{ 0x4D, yasp::Keyboard::Key::M },
	{ 0x4E, yasp::Keyboard::Key::N },
	{ 0x4F, yasp::Keyboard::Key::O },
	{ 0x50, yasp::Keyboard::Key::P },
	{ 0x51, yasp::Keyboard::Key::Q },
	{ 0x52, yasp::Keyboard::Key::R },
	{ 0x53, yasp::Keyboard::Key::S },
	{ 0x54, yasp::Keyboard::Key::T },
	{ 0x55, yasp::Keyboard::Key::U },
	{ 0x56, yasp::Keyboard::Key::V },
	{ 0x57, yasp::Keyboard::Key::W },
	{ 0x58, yasp::Keyboard::Key::X },
	{ 0x59, yasp::Keyboard::Key::Y },
	{ 0x5A, yasp::Keyboard::Key::Z },
	{ 0x30, yasp::Keyboard::Key::NUM_0 },
	{ 0x31, yasp::Keyboard::Key::NUM_1 },
	{ 0x32, yasp::Keyboard::Key::NUM_2 },
	{ 0x33, yasp::Keyboard::Key::NUM_3 },
	{ 0x34, yasp::Keyboard::Key::NUM_4 },
	{ 0x35, yasp::Keyboard::Key::NUM_5 },
	{ 0x36, yasp::Keyboard::Key::NUM_6 },
	{ 0x37, yasp::Keyboard::Key::NUM_7 },
	{ 0x38, yasp::Keyboard::Key::NUM_8 },
	{ 0x39, yasp::Keyboard::Key::NUM_9 },
	{ VK_F1, yasp::Keyboard::Key::F1 },
	{ VK_F2, yasp::Keyboard::Key::F2 },
	{ VK_F3, yasp::Keyboard::Key::F3 },
	{ VK_F4, yasp::Keyboard::Key::F4 },
	{ VK_F5, yasp::Keyboard::Key::F5 },
	{ VK_F6, yasp::Keyboard::Key::F6 },
	{ VK_F7, yasp::Keyboard::Key::F7 },
	{ VK_F8, yasp::Keyboard::Key::F8 },
	{ VK_F9, yasp::Keyboard::Key::F9 },
	{ VK_F10, yasp::Keyboard::Key::F10 },
	{ VK_F11, yasp::Keyboard::Key::F11 },
	{ VK_F12, yasp::Keyboard::Key::F12 },
	{ VK_LSHIFT, yasp::Keyboard::Key::LSHIFT },
	{ VK_RSHIFT, yasp::Keyboard::Key::RSHIFT },
	{ VK_LCONTROL, yasp::Keyboard::Key::LCTRL },
	{ VK_RCONTROL, yasp::Keyboard::Key::RCTRL },
	{ VK_HOME, yasp::Keyboard::Key::HOME },
	{ VK_END, yasp::Keyboard::Key::END },
	{ VK_INSERT, yasp::Keyboard::Key::INS },
	{ VK_DELETE, yasp::Keyboard::Key::DEL },
	{ VK_PRIOR, yasp::Keyboard::Key::PGUP },
	{ VK_NEXT, yasp::Keyboard::Key::PGDOWN },
	{ VK_LEFT, yasp::Keyboard::Key::LEFT },
	{ VK_RIGHT, yasp::Keyboard::Key::RIGHT },
	{ VK_UP, yasp::Keyboard::Key::UP },
	{ VK_DOWN, yasp::Keyboard::Key::DOWN },
	{ VK_BACK, yasp::Keyboard::Key::BACKSPACE },
	{ VK_RETURN, yasp::Keyboard::Key::ENTER },
	{ VK_ESCAPE, yasp::Keyboard::Key::ESCAPE },
	{ VK_SPACE, yasp::Keyboard::Key::SPACE },
	{ VK_CAPITAL, yasp::Keyboard::Key::CAPS },
	{ VK_TAB, yasp::Keyboard::Key::TAB },
	{ VK_SNAPSHOT, yasp::Keyboard::Key::PRTSCR },
	{ VK_MULTIPLY, yasp::Keyboard::Key::NUMPAD_MUL },
	{ VK_ADD, yasp::Keyboard::Key::NUMPAD_PLUS },
	{ VK_SUBTRACT, yasp::Keyboard::Key::NUMPAD_MINUS },
	{ VK_DIVIDE, yasp::Keyboard::Key::NUMPAD_DIV },
	{ VK_DECIMAL, yasp::Keyboard::Key::NUMPAD_COMMA },
	{ VK_OEM_COMMA, yasp::Keyboard::Key::COMMA },
	{ VK_OEM_PERIOD, yasp::Keyboard::Key::PERIOD },
	{ VK_OEM_MINUS, yasp::Keyboard::Key::MINUS },
	{ VK_OEM_PLUS, yasp::Keyboard::Key::PLUS },
	{ VK_OEM_5, yasp::Keyboard::Key::GRAVE },
};



LRESULT yasp::WindowWin32::OnEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE)
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

	WindowWin32* source = reinterpret_cast<WindowWin32*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		source->Close();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		source->Close();
		break;
	case WM_INPUT:
	{
		//https://blog.molecular-matters.com/2011/09/05/properly-handling-keyboard-input/
		char buffer[sizeof(RAWINPUT)] = {};
		uint32_t size = sizeof(RAWINPUT);
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER));
		RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer);
		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			const RAWKEYBOARD& rawKB = raw->data.keyboard;
			UINT virtualKey = rawKB.VKey;
			UINT scanCode = rawKB.MakeCode;
			UINT flags = rawKB.Flags;
			const bool wasUp = ((flags & RI_KEY_BREAK) != 0);
			Keyboard::Key hlKey = Keyboard::Key::DUMMY_KEY;
			if (virtualKey == 255)
			{
				// discard "fake keys" which are part of an escaped sequence
				return 0;
			}
			else if (virtualKey == VK_SHIFT)
			{
				// correct left-hand / right-hand SHIFT
				virtualKey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
			}
			else if (virtualKey == VK_NUMLOCK)
			{
				// correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
				scanCode = (MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) | 0x100);
			}

			const bool isE0 = ((flags & RI_KEY_E0) != 0);
			const bool isE1 = ((flags & RI_KEY_E1) != 0);

			if (isE1)
			{
				if (virtualKey == VK_PAUSE)
					scanCode = 0x45;
				else
					scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
			}
			switch (virtualKey)
			{
				// right-hand CONTROL and ALT have their e0 bit set
			case VK_CONTROL:
				if (isE0)
					hlKey = Keyboard::Key::RCTRL;
				else
					hlKey = Keyboard::Key::LCTRL;
				break;

			case VK_MENU:
				if (isE0)
					hlKey = Keyboard::Key::ALTGR;
				else
					hlKey = Keyboard::Key::ALT;
				break;

				// NUMPAD ENTER has its e0 bit set
			case VK_RETURN:
				if (isE0)
					hlKey = Keyboard::Key::NUMPAD_ENTER;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

				// the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
				// corresponding keys on the NUMPAD will not.
			case VK_INSERT:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_0;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

			case VK_DELETE:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_COMMA;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

			case VK_HOME:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_7;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

			case VK_END:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_1;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

			case VK_PRIOR:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_9;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

			case VK_NEXT:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_3;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

				// the standard arrow keys will always have their e0 bit set, but the
				// corresponding keys on the NUMPAD will not.
			case VK_LEFT:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_4;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

			case VK_RIGHT:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_6;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

			case VK_UP:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_8;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

			case VK_DOWN:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_2;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;

				// NUMPAD 5 doesn't have its e0 bit set
			case VK_CLEAR:
				if (!isE0)
					hlKey = Keyboard::Key::NUMPAD_5;
				else if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;
			default:
				if (auto f = keyMapping.find(virtualKey); f != keyMapping.end())
					hlKey = f->second;
				break;
			}


			//UINT key = (scanCode << 16) | (isE0 << 24);
			//char buffer[512] = {};
			//GetKeyNameText((LONG)key, buffer, 512);
			//std::cout << virtualKey << "\n";
			if (hlKey != Keyboard::Key::DUMMY_KEY)
			{
				if (wasUp)
					Keyboard::ReleaseKey(hlKey);
				else
					Keyboard::PressKey(hlKey);
			}

		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			/*std::cout << "Mouse at?: " << raw->data.mouse.lLastX << ", " << raw->data.mouse.lLastY << "\n";
			std::cout << "What is this?: " << raw->data.mouse.usButtonFlags << "\n";
			std::cout << "What is this?: " << raw->data.mouse.usButtonData << "\n";
			uint32_t buttonevent = raw->data.mouse.usButtonFlags;*/

			switch (raw->data.mouse.usButtonFlags)
			{
			case RI_MOUSE_LEFT_BUTTON_DOWN:
				Mouse::Press(Mouse::Button::LEFT);
				break;
			case RI_MOUSE_LEFT_BUTTON_UP:
				Mouse::Release(Mouse::Button::LEFT);
				break;
			case RI_MOUSE_RIGHT_BUTTON_DOWN:
				Mouse::Press(Mouse::Button::RIGHT);
				break;
			case RI_MOUSE_RIGHT_BUTTON_UP:
				Mouse::Release(Mouse::Button::RIGHT);
				break;
			case RI_MOUSE_MIDDLE_BUTTON_DOWN:
				Mouse::Press(Mouse::Button::MIDDLE);
				break;
			case RI_MOUSE_MIDDLE_BUTTON_UP:
				Mouse::Release(Mouse::Button::MIDDLE);
				break;
			case RI_MOUSE_BUTTON_4_DOWN:
				Mouse::Press(Mouse::Button::X1);
				break;
			case RI_MOUSE_BUTTON_4_UP:
				Mouse::Release(Mouse::Button::X1);
				break;
			case RI_MOUSE_BUTTON_5_DOWN:
				Mouse::Press(Mouse::Button::X2);
				break;
			case RI_MOUSE_BUTTON_5_UP:
				Mouse::Release(Mouse::Button::X2);
				break;
			case RI_MOUSE_WHEEL:
				Mouse::SetScroll(((int16_t)(raw->data.mouse.usButtonData)) / 120);
				break;
			default:
				break;

			}
			switch (raw->data.mouse.usFlags)
			{
			case MOUSE_MOVE_RELATIVE:
				Mouse::SetRelativeMovement({ raw->data.mouse.lLastX , raw->data.mouse.lLastY });
				POINT cp;
				GetCursorPos(&cp);
				ScreenToClient(hwnd, &cp);
				Mouse::SetPos({ (int32_t)cp.x, (int32_t)cp.y });
				break;
			case MOUSE_MOVE_ABSOLUTE:
				Mouse::SetPos({ raw->data.mouse.lLastX , raw->data.mouse.lLastY });
				break;
			default:
				break;
			}
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void yasp::WindowWin32::Close()
{
	open = false;
}

yasp::WindowWin32::WindowWin32(uint32_t width, uint32_t height, bool fullscreen) : width(width), height(height), fullscreen(fullscreen), cursorVisible(true)
{
	hinstance = GetModuleHandle(0);
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WindowWin32::OnEventProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Lye Engine";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		wc.lpszClassName,
		"Lye",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, NULL, hinstance, this);


	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	open = true;

	RAWINPUTDEVICE rawInputDevice[2];
	rawInputDevice[0].usUsagePage = 0x01;
	rawInputDevice[0].usUsage = 0x06; //Keyboard
	rawInputDevice[0].dwFlags = RIDEV_NOLEGACY;
	rawInputDevice[0].hwndTarget = hwnd;

	rawInputDevice[1].usUsagePage = 0x01;
	rawInputDevice[1].usUsage = 0x02; //Mouse
	rawInputDevice[1].dwFlags = fullscreen ? RIDEV_NOLEGACY : 0;
	rawInputDevice[1].hwndTarget = hwnd;
	RegisterRawInputDevices(&rawInputDevice[0], 2, sizeof(RAWINPUTDEVICE));

	Keyboard::Init();
	Mouse::Init();
}

yasp::WindowWin32::~WindowWin32()
{

}

void * yasp::WindowWin32::GetWindowHandle()
{
	return hwnd;
}

void yasp::WindowWin32::SetTitle(const std::string & title)
{
	SetWindowText(hwnd, title.c_str());
}

bool yasp::WindowWin32::IsOpen() const
{
	return open;
}

void yasp::WindowWin32::SetCursorVisibility(bool visible)
{
	if (cursorVisible != visible)
	{
		ShowCursor(visible);
		cursorVisible = visible;
	}
}

void yasp::WindowWin32::SetWindowVisibility(bool visible)
{
	ShowWindow(hwnd, visible ? SW_SHOW : SW_HIDE);
}

void yasp::WindowWin32::SetGrabCursor(bool grabbing)
{
	//todo...
}

int32_t yasp::WindowWin32::GetWidth() const
{
	return height;
}

int32_t yasp::WindowWin32::GetHeight() const
{
	return width;
}

float yasp::WindowWin32::GetAspectRatio() const
{
	return (float)width / height;
}

void yasp::WindowWin32::PollEvents()
{
	Keyboard::Update(std::chrono::high_resolution_clock::now());
	Mouse::Update(std::chrono::high_resolution_clock::now());
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
}

#endif //LYE_WIN32