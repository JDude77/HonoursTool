#include "NuklearWindow.h"

NuklearWindow::NuklearWindow(WINDOW_TYPE windowType, shared_ptr<PrimaryData> windowData) : windowData_(std::move(windowData)), windowType_(windowType)
{
	switch (windowType_)
	{
	case NONE: 
		hasHeader_ = false;
		hasFooter_ = false;
		windowTitle_ = "WINDOW IS NONETYPE";
		break;

	case LANDING:
		hasHeader_ = false;
		hasFooter_ = false;
		windowTitle_ = "Data Authoring Tool - Press Button To Open Window";
		break;

	case MEMBER_WINDOW:
		hasHeader_ = true;
		hasFooter_ = true;
		windowTitle_ = "Member Creator";
		break;

	case TEMPLATE_WINDOW:
		hasHeader_ = true;
		hasFooter_ = true;
		windowTitle_ = "Template Creator";
		break;
	
	case GROUP_WINDOW: 
		hasHeader_ = true;
		hasFooter_ = true;
		windowTitle_ = "Group Creator";
		break;

	default:
		
		break;
	}//End switch
}//End constructor