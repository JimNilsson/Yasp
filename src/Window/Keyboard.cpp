#include <Yasp/Window/Keyboard.h>
#include <unordered_map>
yasp::Keyboard::KeyStatus yasp::Keyboard::keyStatus[KEY_COUNT];
float yasp::Keyboard::doubletapThreshold = 0.25f;
std::chrono::high_resolution_clock::time_point yasp::Keyboard::currentTime;
std::chrono::high_resolution_clock::time_point yasp::Keyboard::keyReleasedAt[yasp::Keyboard::KEY_COUNT];
std::chrono::high_resolution_clock::time_point yasp::Keyboard::keyPressedAt[yasp::Keyboard::KEY_COUNT];



bool yasp::Keyboard::IsKeyDown(Key key)
{
	return keyStatus[key] & KeyStatus::STATUS_DOWN;
}

bool yasp::Keyboard::WasKeyPressed(Key key)
{
	return keyStatus[key] & KeyStatus::STATUS_SINGLE_TAP;
}

bool yasp::Keyboard::WasKeyReleased(Key key)
{
	return keyStatus[key] & KeyStatus::STATUS_RELEASED;
}

bool yasp::Keyboard::WasKeyDoubleTapped(Key key)
{
	return keyStatus[key] & KeyStatus::STATUS_DOUBLE_TAP;
}

const char * yasp::Keyboard::KeyName(Key key)
{
	static std::unordered_map<Key, const char*> keyNames =
	{
		{ Key::DUMMY_KEY, "Key::DUMMY_KEY" },
		{ Key::A, "Key::A" },
		{ Key::B, "Key::B" },
		{ Key::C, "Key::C" },
		{ Key::D, "Key::D" },
		{ Key::E, "Key::E" },
		{ Key::F, "Key::F" },
		{ Key::G, "Key::G" },
		{ Key::H, "Key::H" },
		{ Key::I, "Key::I" },
		{ Key::J, "Key::J" },
		{ Key::K, "Key::K" },
		{ Key::L, "Key::L" },
		{ Key::M, "Key::M" },
		{ Key::N, "Key::N" },
		{ Key::O, "Key::O" },
		{ Key::P, "Key::P" },
		{ Key::Q, "Key::Q" },
		{ Key::R, "Key::R" },
		{ Key::S, "Key::S" },
		{ Key::T, "Key::T" },
		{ Key::U, "Key::U" },
		{ Key::V, "Key::V" },
		{ Key::W, "Key::W" },
		{ Key::X, "Key::X" },
		{ Key::Y, "Key::Y" },
		{ Key::Z, "Key::Z" },
		{ Key::NUM_0,"Key::NUM_0" },
		{ Key::NUM_1,"Key::NUM_1" },
		{ Key::NUM_2,"Key::NUM_2" },
		{ Key::NUM_3,"Key::NUM_3" },
		{ Key::NUM_4,"Key::NUM_4" },
		{ Key::NUM_5,"Key::NUM_5" },
		{ Key::NUM_6,"Key::NUM_6" },
		{ Key::NUM_7,"Key::NUM_7" },
		{ Key::NUM_8,"Key::NUM_8" },
		{ Key::NUM_9,"Key::NUM_9" },
		{ Key::NUMPAD_0,"Key::NUMPAD_0" },
		{ Key::NUMPAD_1,"Key::NUMPAD_1" },
		{ Key::NUMPAD_2,"Key::NUMPAD_2" },
		{ Key::NUMPAD_3,"Key::NUMPAD_3" },
		{ Key::NUMPAD_4,"Key::NUMPAD_4" },
		{ Key::NUMPAD_5,"Key::NUMPAD_5" },
		{ Key::NUMPAD_6,"Key::NUMPAD_6" },
		{ Key::NUMPAD_7,"Key::NUMPAD_7" },
		{ Key::NUMPAD_8,"Key::NUMPAD_8" },
		{ Key::NUMPAD_9,"Key::NUMPAD_9" },
		{ Key::NUMPAD_ENTER, "Key::NUMPAD_ENTER" },
		{ Key::NUMPAD_DIV,"Key::NUMPAD_DIV" },
		{ Key::NUMPAD_MUL,"Key::NUMPAD_MUL" },
		{ Key::NUMPAD_PLUS, "Key::NUMPAD_PLUS" },
		{ Key::NUMPAD_MINUS, "Key::NUMPAD_MINUS" },
		{ Key::NUMPAD_COMMA, "Key::NUMPAD_COMMA" },
		{ Key::LSHIFT,"Key::LSHIFT" },
		{ Key::RSHIFT,"Key::RSHIFT" },
		{ Key::LCTRL,"Key::LCTRL" },
		{ Key::RCTRL,"Key::RCTRL" },
		{ Key::ALT,"Key::ALT" },
		{ Key::ALTGR,"Key::ALTGR" },
		{ Key::TAB,"Key::TAB" },
		{ Key::CAPS,"Key::CAPS" },
		{ Key::GRAVE,"Key::GRAVE" },
		{ Key::F1,"Key::F1" },
		{ Key::F2,"Key::F2" },
		{ Key::F3,"Key::F3" },
		{ Key::F4,"Key::F4" },
		{ Key::F5,"Key::F5" },
		{ Key::F6,"Key::F6" },
		{ Key::F7,"Key::F7" },
		{ Key::F8,"Key::F8" },
		{ Key::F9,"Key::F9" },
		{ Key::F10,"Key::F10" },
		{ Key::F11,"Key::F11" },
		{ Key::F12,"Key::F12" },
		{ Key::LEFT,"Key::LEFT" },
		{ Key::RIGHT,"Key::RIGHT" },
		{ Key::UP,"Key::UP" },
		{ Key::DOWN,"Key::DOWN" },
		{ Key::PRTSCR,"Key::PRTSCR" },
		{ Key::INS,"Key::INS" },
		{ Key::HOME,"Key::HOME" },
		{ Key::END,"Key::END" },
		{ Key::PGUP,"Key::PGUP" },
		{ Key::PGDOWN,"Key::PGDOWN" },
		{ Key::DEL,"Key::DEL" },
		{ Key::ENTER,"Key::ENTER" },
		{ Key::BACKSPACE,"Key::BACKSPACE" },
		{ Key::SPACE,"Key::SPACE" },
		{ Key::ESCAPE,"Key::ESCAPE" },
		{ Key::COMMA,"Key::COMMA" },
		{ Key::PERIOD,"Key::PERIOD" },
		{ Key::MINUS,"Key::MINUS" },
		{ Key::PLUS,"Key::PLUS" }
	};
	if (auto f = keyNames.find(key); f != keyNames.end())
		return f->second;
	return nullptr;
}

void yasp::Keyboard::PressKey(Key key)
{
	auto kr = keyReleasedAt[key];
	auto kp = keyPressedAt[key];
	if (kr >= kp)
	{
		keyPressedAt[key] = currentTime;
		auto secondsSinceLastPress = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - kr).count();
		if (secondsSinceLastPress < doubletapThreshold)
			keyStatus[key] = static_cast<KeyStatus>(KeyStatus::STATUS_DOUBLE_TAP | KeyStatus::STATUS_SINGLE_TAP);
		else
			keyStatus[key] = KeyStatus::STATUS_SINGLE_TAP;
	}
	else
	{
		keyStatus[key] = KeyStatus::STATUS_DOWN;
	}
}

void yasp::Keyboard::ReleaseKey(Key key)
{
	keyReleasedAt[key] = currentTime;
	keyStatus[key] = KeyStatus::STATUS_RELEASED;
}

void yasp::Keyboard::Update(std::chrono::high_resolution_clock::time_point curtime)
{
	currentTime = curtime;
	for (auto& status : keyStatus)
	{
		if (status & KeyStatus::STATUS_RELEASED)
			status = KeyStatus::STATUS_UP;
		if (status & (KeyStatus::STATUS_DOUBLE_TAP | KeyStatus::STATUS_SINGLE_TAP))
			status = KeyStatus::STATUS_DOWN;
	}
}

void yasp::Keyboard::Init()
{
	for (auto& status : keyStatus)
		status = KeyStatus::STATUS_UP;
	currentTime = std::chrono::high_resolution_clock::now();
}
