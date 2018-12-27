#include <Yasp/Window/Window.h>
#include <Yasp/Rendering/RenderContext.h>
#include <Yasp/Window/Keyboard.h>
#include <Yasp/Window/Mouse.h>
#include <iostream>
int main(int argc, char** argv)
{
	yasp::Window window(640, 480);
	yasp::RenderContext renderContext(window.GetWindowHandle());
	while (window.IsOpen())
	{
		window.PollEvents();
		if (yasp::Keyboard::IsKeyDown(yasp::Keyboard::Key::A))
		{
			std::cout << "A is down\n";
		}
		if (yasp::Keyboard::WasKeyDoubleTapped(yasp::Keyboard::Key::B))
		{
			std::cout << "B was dtapped\n";
		}
	}
	return 0;
}