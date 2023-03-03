#include "Renderer/Renderer.h"
#include "Data/DataManager.h"

int main()
{
	static int running = 1;
	shared_ptr<DataManager> dataManager = std::make_shared<DataManager>(new DataManager());
	auto renderer = new Renderer("Demo", 600, 400, dataManager, "", 12);

	while(running)
	{
		running = renderer->Update();
	}//End running

	delete renderer;
	renderer = nullptr;
}//End main