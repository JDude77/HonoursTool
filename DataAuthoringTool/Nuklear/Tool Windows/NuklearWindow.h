#pragma once
#ifndef NUKLEAR_WINDOW
#define NUKLEAR_WINDOW
#include <memory>
#include <queue>
#include <string>
using std::shared_ptr;

enum struct WINDOW_TYPE
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
	NuklearWindow(WINDOW_TYPE windowType, const shared_ptr<DataManager>& dataManager, const shared_ptr<PrimaryData>& windowData = nullptr, const char* windowTitle = nullptr);
	~NuklearWindow();
	bool UpdateWindowTitle(const char* newWindowTitle);
	[[nodiscard]] const char* GetWindowTitle() const { return windowTitle_; }
	shared_ptr<PrimaryData> GetWindowData() { return windowData_; }
	[[nodiscard]] WINDOW_TYPE GetWindowType() const { return windowType_; }
	[[nodiscard]] std::queue<std::string>* GetSubFooterText() const { return subFooterText_.get(); }
	[[nodiscard]] bool GetHasHeader() const { return hasHeader_; }
	[[nodiscard]] bool GetHasFooter() const { return hasFooter_; }
	[[nodiscard]] bool GetHasSubFooter() const { return hasSubFooter_; }
	void ClearSubFooterText() const { while(!subFooterText_->empty()) subFooterText_->pop(); }

private:
	bool hasHeader_, hasFooter_, hasSubFooter_;
	shared_ptr<PrimaryData> windowData_ = nullptr;
	const char* windowTitle_;
	std::unique_ptr<std::queue<std::string>> subFooterText_ = std::make_unique<std::queue<std::string>>();
	WINDOW_TYPE windowType_ = WINDOW_TYPE::NONE;
};
#endif