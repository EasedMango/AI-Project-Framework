#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include "Core.h"
#include "MemoryManager.h"
#include "MemoryScene.h"
#include "MemoryTracker.h"
//#include <vld.h>
class TestScene;



//main loop
int main(int argc, char* argv[])
{
	
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	MemoryManager::Init(MEGA_BYTE* 200);

	{

		const auto core = std::make_unique<Core>();

		core->Initialize("TestScene", 1280, 720);

		core->BuildNewScene<TestScene>();

		core->Run();
	}
//	MemoryManager::CleanUp();
	return 0;
}