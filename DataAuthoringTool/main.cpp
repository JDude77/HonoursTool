#include "Renderer/Renderer.h"
#include "Data/DataManager.h"

int main()
{
	static int running = 1;
	shared_ptr<DataManager> dataManager = std::make_shared<DataManager>(DataManager());
	auto renderer = new Renderer("Demo", 1280, 720, std::move(dataManager), "", 12);

	while(running)
	{
		running = renderer->Update();
	}//End running

	delete renderer;
	renderer = nullptr;
}//End main