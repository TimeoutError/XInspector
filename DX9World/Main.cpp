#include "DXWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR CmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(CmdLine);

	cDXWindow wEnt {
		hInstance,
		640,
		480
	};

	std::string wTitle = "Inspector";

	if (wEnt.Prepare(wTitle))
	{
		wEnt.Setup();
		wEnt.Run();
	}
	else
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}