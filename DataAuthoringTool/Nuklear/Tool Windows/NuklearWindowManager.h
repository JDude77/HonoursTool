#pragma once
#ifndef NUKLEAR_WINDOW_MANAGER
#define NUKLEAR_WINDOW_MANAGER

#include <vector>
using std::vector;

//Forward Declarations
class NuklearWindow;
struct nk_rect;
struct nk_context;

class NuklearWindowManager
{
public:
	NuklearWindowManager();
	bool RenderAllActiveWindows(nk_context* nuklearContext);
	bool CreateNewWindow(char* windowTitle, struct nk_rect windowStartSize);

private:
	vector<NuklearWindow*> actvieNuklearWindows_;
	bool RenderWindow(NuklearWindow* nuklearWindow);
};
#endif