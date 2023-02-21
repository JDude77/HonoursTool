#include "NuklearWindowManager.h"

struct nk_rect { float x, y, w, h; };

NuklearWindowManager::NuklearWindowManager()
{
}

bool NuklearWindowManager::RenderAllActiveWindows(nk_context* nuklearContext)
{
	return true;
}

bool NuklearWindowManager::CreateNewWindow(char* windowTitle, nk_rect windowStartSize)
{
	return true;
}

bool NuklearWindowManager::RenderWindow(NuklearWindow* nuklearWindow)
{
	return true;
}