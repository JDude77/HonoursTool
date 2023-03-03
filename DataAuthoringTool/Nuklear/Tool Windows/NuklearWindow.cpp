#include "NuklearWindow.h"

NuklearWindow::NuklearWindow(WINDOW_TYPE windowType, shared_ptr<PrimaryData> windowData) : windowType_(windowType), windowData_(windowData)
{
	switch (windowType_)
	{
	case WINDOW_TYPE::LANDING:

		break;

	case WINDOW_TYPE::MEMBER_WINDOW:

		break;

	case WINDOW_TYPE::TEMPLATE_WINDOW:

		break;
	}//End switch
}//End constructor