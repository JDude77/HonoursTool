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
class DataManager;

struct NuklearWindow
{
	NuklearWindow(WINDOW_TYPE windowType, shared_ptr<DataManager> dataManager, shared_ptr<PrimaryData> windowData = nullptr, const char* windowTitle = nullptr);
	~NuklearWindow();
	bool UpdateWindowTitle(const char* newWindowTitle);
	[[nodiscard]] const char* GetWindowTitle() const { return windowTitle_; }
	shared_ptr<PrimaryData> GetWindowData() { return windowData_; }
	WINDOW_TYPE GetWindowType() const { return windowType_; }
	bool GetHasHeader() const { return hasHeader_; }
	bool GetHasFooter() const { return hasFooter_; }

private:
	bool hasHeader_, hasFooter_;
	shared_ptr<PrimaryData> windowData_ = nullptr;
	const char* windowTitle_;
	WINDOW_TYPE windowType_ = NONE;
};
#endif