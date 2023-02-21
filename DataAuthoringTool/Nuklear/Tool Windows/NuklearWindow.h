#pragma once
#ifndef NUKLEAR_WINDOW
#define NUKLEAR_WINDOW

enum WINDOW_TYPE
{
	NONE = -1,
	MEMBER,
	GROUP,
	TEMPLATE,
	LANDING
};

struct NuklearWindow
{
	NuklearWindow(const char* windowTitle, const WINDOW_TYPE windowType);
	~NuklearWindow();
	bool UpdateWindowTitle(const char* newWindowTitle);

private:
	char* windowTitle_;
	WINDOW_TYPE windowType_ = NONE;
};
#endif