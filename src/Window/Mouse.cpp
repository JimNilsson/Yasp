#include <Yasp/Window/Mouse.h>

int32_t yasp::Mouse::xpos;
int32_t yasp::Mouse::ypos;
int32_t yasp::Mouse::relx;
int32_t yasp::Mouse::rely;
int32_t yasp::Mouse::scroll;

float yasp::Mouse::doubleclickThreshold = 0.25f;
int32_t yasp::Mouse::previousx;
int32_t yasp::Mouse::previousy;
std::chrono::high_resolution_clock::time_point yasp::Mouse::currentTime;
yasp::Mouse::ButtonStatus yasp::Mouse::buttonStatus[BUTTON_COUNT];
std::chrono::high_resolution_clock::time_point yasp::Mouse::buttonReleasedAt[BUTTON_COUNT];
std::chrono::high_resolution_clock::time_point yasp::Mouse::buttonPressedAt[BUTTON_COUNT];

void yasp::Mouse::SetPos(const vec2<int32_t>& pos)
{
	xpos = pos.x;
	ypos = pos.y;
}

void yasp::Mouse::SetRelativeMovement(const vec2<int32_t>& pos)
{
	relx = pos.x;
	rely = pos.y;
}

void yasp::Mouse::Press(Button button)
{
	auto kr = buttonReleasedAt[button];
	auto kp = buttonPressedAt[button];
	if (kr >= kp)
	{
		buttonPressedAt[button] = currentTime;
		auto secondsSinceLastPress = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - kr).count();
		if (secondsSinceLastPress < doubleclickThreshold)
			buttonStatus[button] = static_cast<ButtonStatus>(ButtonStatus::STATUS_DOUBLE_TAP | ButtonStatus::STATUS_SINGLE_TAP);
		else
			buttonStatus[button] = ButtonStatus::STATUS_SINGLE_TAP;
	}
	else
	{
		buttonStatus[button] = ButtonStatus::STATUS_DOWN;
	}
}

void yasp::Mouse::Release(Button button)
{
	buttonReleasedAt[button] = currentTime;
	buttonStatus[button] = ButtonStatus::STATUS_RELEASED;
}

void yasp::Mouse::SetScroll(int32_t scroll)
{
	Mouse::scroll = scroll;
}

yasp::vec2<int32_t> yasp::Mouse::GetPos()
{
	return { xpos, ypos };
}

yasp::vec2<int32_t> yasp::Mouse::GetRelativeMovement()
{

	return { relx, rely };
}

bool yasp::Mouse::WasClicked(Button button)
{
	return buttonStatus[button] & ButtonStatus::STATUS_SINGLE_TAP;
}

bool yasp::Mouse::WasDoubleClicked(Button button)
{
	return buttonStatus[button] & ButtonStatus::STATUS_DOUBLE_TAP;
}

bool yasp::Mouse::WasReleased(Button button)
{
	return buttonStatus[button] & ButtonStatus::STATUS_RELEASED;
}

int32_t yasp::Mouse::GetScroll()
{
	return scroll;
}

void yasp::Mouse::Update(std::chrono::high_resolution_clock::time_point curtime)
{
	currentTime = curtime;
	relx = 0;
	rely = 0;
	previousx = xpos;
	previousy = ypos;
	scroll = 0;
	for (auto& status : buttonStatus)
	{
		if (status & ButtonStatus::STATUS_RELEASED)
			status = ButtonStatus::STATUS_UP;
		if (status & (ButtonStatus::STATUS_DOUBLE_TAP | ButtonStatus::STATUS_SINGLE_TAP))
			status = ButtonStatus::STATUS_DOWN;
	}
}

void yasp::Mouse::Init()
{
	for (auto& status : buttonStatus)
		status = ButtonStatus::STATUS_UP;
	currentTime = std::chrono::high_resolution_clock::now();
}

bool yasp::Mouse::IsDown(Button button)
{
	return buttonStatus[button] & ButtonStatus::STATUS_DOWN;
}