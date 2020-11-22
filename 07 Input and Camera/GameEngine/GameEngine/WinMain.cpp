#include <Windows.h>

#include "Application.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nShowCmd)
{
	Application app;

	if (app.Initialize (L"Game Engine", 600, 600, false))
	{
		app.Update ();
	}

	app.Shutdown ();

	return 0;
}
