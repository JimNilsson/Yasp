#ifndef YASP_MOUSE_H
#define YASP_MOUSE_H
#include <cstdint>
#include <chrono>
#include <Yasp/Math.h>
namespace yasp
{
	class Mouse
	{
	public:
		enum Button : uint32_t
		{
			LEFT,
			RIGHT,
			MIDDLE,
			X1,
			X2,
			BUTTON_COUNT
		};

		static vec2<int32_t> GetPos();
		static vec2<int32_t> GetRelativeMovement();

		static bool IsDown(Button button);
		static bool WasClicked(Button button);
		static bool WasDoubleClicked(Button button);
		static bool WasReleased(Button button);
		static int32_t GetScroll();

		static void SetPos(const vec2<int32_t>& pos);
		static void SetRelativeMovement(const vec2<int32_t>& pos);
		static void Press(Button button);
		static void Release(Button button);
		static void SetScroll(int32_t scroll);

		static void Update(std::chrono::high_resolution_clock::time_point curtime);
		static void Init();
	private:
		enum ButtonStatus : uint8_t
		{
			STATUS_UP = 1U << 0U,
			STATUS_DOWN = 1U << 1U,
			STATUS_RELEASED = 1U << 2U,
			STATUS_SINGLE_TAP = 1U << 3U,
			STATUS_DOUBLE_TAP = 1U << 4U
		};

		static int32_t xpos;
		static int32_t ypos;
		static int32_t relx;
		static int32_t rely;

		static int32_t scroll;

		static float doubleclickThreshold;
		static int32_t previousx;
		static int32_t previousy;
		static std::chrono::high_resolution_clock::time_point currentTime;
		static ButtonStatus buttonStatus[BUTTON_COUNT];
		static std::chrono::high_resolution_clock::time_point buttonReleasedAt[BUTTON_COUNT];
		static std::chrono::high_resolution_clock::time_point buttonPressedAt[BUTTON_COUNT];
	};
}

#endif
