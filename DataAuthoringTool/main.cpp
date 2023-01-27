#include "Renderer/Renderer.h"

int main()
{
	static int running = 1;
	auto renderer = new Renderer("Demo", 400, 300);

	while(running)
	{
		running = renderer->Update();
	}//End running

	delete renderer;
	renderer = nullptr;
}//End main