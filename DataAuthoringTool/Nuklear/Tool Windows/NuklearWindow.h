#pragma once
#ifndef NUKLEAR_WINDOW
#define NUKLEAR_WINDOW
#include <memory>
using std::shared_ptr;

enum WINDOW_TYPE
{
	NONE = -1,
	MEMBER_WINDOW,
	GROUP_WINDOW,
	TEMPLATE_WINDOW,
	LANDING
};

class PrimaryData;

struct NuklearWindow
{
	NuklearWindow(WINDOW_TYPE windowType, shared_ptr<PrimaryData> windowData);
	~NuklearWindow();
	bool UpdateWindowTitle(const char* newWindowTitle);
	const char* GetWindowTitle() { return windowTitle_; }
	shared_ptr<PrimaryData> GetWindowData() { return windowData_; }

private:
	shared_ptr<PrimaryData> windowData_ = nullptr;
	const char* windowTitle_;
	WINDOW_TYPE windowType_ = NONE;
};
#endif