#include <Yasp/Window/Window.h>

int main(int argc, char** argv)
{
	yasp::Window window(640, 480);
	while (window.IsOpen())
	{
		window.PollEvents();
	}
	return 0;
}