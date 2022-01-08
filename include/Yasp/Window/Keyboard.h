#ifndef YASP_KEYBOARD_H
#define YASP_KEYBOARD_H

#include <Yasp/Config.h>
#include <string>
#include <chrono>
namespace yasp
{
	class Keyboard
	{
	public:
		enum Key : uint32
		{
			DUMMY_KEY,
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			NUM_0,
			NUM_1,
			NUM_2,
			NUM_3,
			NUM_4,
			NUM_5,
			NUM_6,
			NUM_7,
			NUM_8,
			NUM_9,
			NUMPAD_0,
			NUMPAD_1,
			NUMPAD_2,
			NUMPAD_3,
			NUMPAD_4,
			NUMPAD_5,
			NUMPAD_6,
			NUMPAD_7,
			NUMPAD_8,
			NUMPAD_9,
			NUMPAD_ENTER,
			NUMPAD_DIV,
			NUMPAD_MUL,
			NUMPAD_PLUS,
			NUMPAD_MINUS,
			NUMPAD_COMMA,
			LSHIFT,
			RSHIFT,
			LCTRL,
			RCTRL,
			ALT,
			ALTGR,
			TAB,
			CAPS,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			LEFT,
			RIGHT,
			UP,
			DOWN,
			PRTSCR,
			INS,
			HOME,
			END,
			PGUP,
			PGDOWN,
			DEL,
			ENTER,
			BACKSPACE,
			SPACE,
			ESCAPE,
			GRAVE,
			COMMA,
			PERIOD,
			MINUS,
			PLUS,
			KEY_COUNT
		};
		static bool IsKeyDown(Key key);
		static bool WasKeyPressed(Key key);
		static bool WasKeyReleased(Key key);
		static bool WasKeyDoubleTapped(Key key);

		static const char* KeyName(Key key);

		static void AddInput(std::string&& input);
		static std::string GetInput();
		static void ClearInput();
		static void PressKey(Key key);
		static void ReleaseKey(Key key);
		static void Update(std::chrono::high_resolution_clock::time_point curtime);
		static void Init();
	private:
		enum KeyStatus : uint8_t
		{
			STATUS_UP = 1U << 0U,
			STATUS_DOWN = 1U << 1U,
			STATUS_RELEASED = 1U << 2U,
			STATUS_SINGLE_TAP = 1U << 3U,
			STATUS_DOUBLE_TAP = 1U << 4U
		};
		static float doubletapThreshold;
		static std::chrono::high_resolution_clock::time_point currentTime;
		static KeyStatus keyStatus[KEY_COUNT];
		static std::chrono::high_resolution_clock::time_point keyReleasedAt[KEY_COUNT];
		static std::chrono::high_resolution_clock::time_point keyPressedAt[KEY_COUNT];
		static std::string input;
	};
}


#endif
