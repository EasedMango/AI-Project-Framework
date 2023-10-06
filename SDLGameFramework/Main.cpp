
#include "Core.h"

class TestScene;
//main loop
int main(int argc, char *argv[])
{
	{
		const auto core = std::make_unique<Core>();

		core->Initialize("TestScene", 1280, 720);

		core->BuildNewScene<TestScene>();

		core->Run();
	}

	return 0;
}