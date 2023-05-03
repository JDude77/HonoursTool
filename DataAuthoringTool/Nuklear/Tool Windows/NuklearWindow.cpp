#include "NuklearWindow.h"

#include "../../Data/DataManager.h"
#include "../../Data/Members/Member.h"
#include "../../Data/Templates/Template.h"
#include "../../Data/Groups/Group.h"
#include "../../Data/Shared/DataInterfaces.h"

using std::dynamic_pointer_cast;

NuklearWindow::NuklearWindow(const WINDOW_TYPE windowType, const shared_ptr<DataManager>& dataManager, const shared_ptr<PrimaryData>& windowData, const char* windowTitle) : windowData_(windowData), windowType_(windowType)
{
	switch (windowType_)
	{
	case WINDOW_TYPE::NONE: 
		hasHeader_ = false;
		hasFooter_ = false;
		hasSubFooter_ = false;
		windowTitle_ = "WINDOW IS NONETYPE";
		break;

	case WINDOW_TYPE::LANDING:
		hasHeader_ = false;
		hasFooter_ = false;
		hasSubFooter_ = false;
		windowTitle_ = windowTitle == nullptr ? "Data Authoring Tool - Press Button To Open Window" : windowTitle;
		break;

	case WINDOW_TYPE::MEMBER_WINDOW:
		hasHeader_ = true;
		hasFooter_ = true;
		hasSubFooter_ = true;
		windowTitle_ = windowTitle == nullptr ? "Member Creator" : windowTitle;
		{
			const auto data = dynamic_pointer_cast<Member>(windowData);
			dataManager->AddInstanceToDataMap(Member(*data));
		}//End pointer cast
		break;

	case WINDOW_TYPE::TEMPLATE_WINDOW:
		hasHeader_ = true;
		hasFooter_ = true;
		hasSubFooter_ = true;
		windowTitle_ = windowTitle == nullptr ? "Template Creator" : windowTitle;
		{
			const auto data = dynamic_pointer_cast<Template>(windowData);

			dataManager->AddInstanceToDataMap(Template(*data));
		}//End pointer cast
		break;
	
	case WINDOW_TYPE::GROUP_WINDOW: 
		hasHeader_ = true;
		hasFooter_ = true;
		hasSubFooter_ = true;
		windowTitle_ = windowTitle == nullptr ? "Group Creator" : windowTitle;
		{
			const auto data = dynamic_pointer_cast<Group>(windowData);
			dataManager->AddInstanceToDataMap(Group(*data));
		}//End pointer cast
		break;
	}//End switch
}//End constructor