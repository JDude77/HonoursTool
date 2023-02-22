#pragma once
#ifndef NUKLEAR_WINDOW
#define NUKLEAR_WINDOW
#include <memory>
using std::shared_ptr;

enum WINDOW_TYPE
{
	NONE = -1,
	MEMBER,
	GROUP,
	TEMPLATE,
	LANDING
};

class PrimaryData;

struct NuklearWindow
{
	NuklearWindow(const char* windowTitle, PrimaryData* windowData);
	~NuklearWindow();
	bool UpdateWindowTitle(const char* newWindowTitle);
	char* GetWindowTitle() { return windowTitle_; }
	shared_ptr<PrimaryData> GetWindowData() { return windowData_; }

private:
	shared_ptr<PrimaryData> windowData_ = nullptr;
	char* windowTitle_;
	WINDOW_TYPE windowType_ = NONE;
};
#endif