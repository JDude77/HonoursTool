#pragma once
//Header-only class to take advantage of nuklear.h being declared earlier in the program execution, meaning this class has access to its functions
//Not ideal, but avoids re-definition errors with Nuklear that would otherwise crop up
#ifndef NUKLEAR_WINDOW_MANAGER
#define NUKLEAR_WINDOW_MANAGER
#include "NuklearWindow.h"
#include <vector>

#include "../../Data/Shared/DataInterfaces.h"
#include "../../Data/Members/Member.h"
#include "../../Data/DataManager.h"
#include "../../Data/Groups/Group.h"
#include "../../Data/Templates/Template.h"
using std::vector;
using std::dynamic_pointer_cast;
using std::make_shared;

class NuklearWindowManager
{
private:
	static shared_ptr<vector<Template>> cachedTemplates_;
	shared_ptr<DataManager> dataManager_;
	vector<NuklearWindow*> activeNuklearWindows_;

	void RenderTemplateHeaderDropdown(nk_context* nuklearContext, const shared_ptr<Member>& memberData)
	{
		//Update cached templates
		//TODO: Refactor this to not happen every frame of rendering a member window - only on templates updating
		cachedTemplates_ = make_shared<vector<Template>>(dataManager_->GetAllTemplates());

		vector<char*> templateNames;
		for (auto& temp : *cachedTemplates_)
		{
			templateNames.push_back(temp.GetNameBuffer());
		}//End for
		
		//Render templates dropdown
		nk_combo(nuklearContext, const_cast<const char**>(templateNames.data()), NK_LEN(templateNames), memberData->GetTemplateIndex(), 24, nk_vec2(300, 300));
	}//End RenderTemplateHeaderDropdown

	bool RenderWindowHeader(nk_context* nuklearContext, const shared_ptr<PrimaryData>& windowData)
	{
		nk_layout_row_dynamic(nuklearContext, 24, 4);
			//Name
			nk_label(nuklearContext, "Name: ", NK_TEXT_LEFT);
			nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, windowData->GetNameBuffer(), windowData->GetNameBufferCurrentLength(), windowData->GetBufferMax(), nk_filter_default);
			//ID
			nk_label(nuklearContext, "ID: ", NK_TEXT_LEFT);
			nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, windowData->GetIDBuffer(), windowData->GetIDBufferCurrentLength(), windowData->GetBufferMax(), nk_filter_default);

		//If the data we're handling is a member
		if(const auto memberData = dynamic_pointer_cast<Member>(windowData))
		{
			//Member/Template Type dropdown
			nk_layout_row_dynamic(nuklearContext, 24, 1);
				nk_label(nuklearContext, "Member Template/Type", NK_TEXT_LEFT);
			nk_layout_row_dynamic(nuklearContext, 24, 2);
				RenderTemplateHeaderDropdown(nuklearContext, memberData);
				//Validate All Fields button
				if(nk_button_label(nuklearContext, "VALIDATE ALL")) memberData->Validate();
		}//End if

		//If the data we're handling is a group
		else if(const auto groupData = dynamic_pointer_cast<Group>(windowData))
		{
			//Validate All Members button
			nk_layout_row_dynamic(nuklearContext, 24, 1);
				if(nk_button_label(nuklearContext, "VALIDATE")) groupData->Validate();
		}//End else if

		return true;
	}//End RenderWindowHeader

	bool RenderWindowFooter(nk_context* nuklearContext, const shared_ptr<PrimaryData>& windowData)
	{
		nk_layout_row_dynamic(nuklearContext, 24, 1);
			if(nk_button_label(nuklearContext, "SAVE")) windowData->Save();
			if(nk_button_label(nuklearContext, "LOAD")) windowData->Load();
			if(nk_button_label(nuklearContext, "EXPORT")) windowData->Export();
			if(nk_button_label(nuklearContext, "DELETE")) windowData->Delete();

		return true;
	}//End RenderWindowFooter

	bool RenderWindowBody(nk_context* nuklearContext, const shared_ptr<PrimaryData>& windowData)
	{
		//LANDING
		if(windowData == nullptr)
		{
			//Template Button

			//Member Button

			//Group Button
		}//End if

		//MEMBER
		else if(const auto memberData = dynamic_pointer_cast<Member>(windowData))
		{
			//For Each Field
				//1 Column Row

					//Name of field label followed by type

				//3 Column Row

					//Input field for data

					//Delete/Clear button for field

					//Validate button for field

		}//End else if

		//GROUP
		else if(const auto groupData = dynamic_pointer_cast<Group>(windowData))
		{
			//Future me problem
		}//End else if

		//TEMPLATE
		else if (const auto templateData = dynamic_pointer_cast<Template>(windowData))
		{
			//For Each Field

				//Field Name

				//Field Type

				//Validation Rules

				//Delete Field Button

			//Add Field Button
		}//End else if

		return false;
	}//End RenderWindowBody

	bool RenderWindow(nk_context* nuklearContext, NuklearWindow* nuklearWindow)
	{
		shared_ptr<PrimaryData> windowData = nuklearWindow->GetWindowData();
		if(windowData != nullptr)
		{
			if(nk_begin(nuklearContext, nuklearWindow->GetWindowTitle(), nk_rect(25, 50, 100, 100),
		    NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
		    NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE))
			{
				//Render Header
				if(nuklearWindow->GetHasHeader())
				{
					if(!RenderWindowHeader(nuklearContext, windowData)) return false;
				}//End if

				//Render Body
				if(!RenderWindowBody(nuklearContext, windowData)) return false;

				//Render Footer
				if(nuklearWindow->GetHasFooter())
				{
					if(!RenderWindowFooter(nuklearContext, windowData)) return false;
				}//End if
			}//End if
			nk_end(nuklearContext);
		}//End if

		//Landing window has no data, as it is essentially just buttons to open subwindows
		else if(nuklearWindow->GetWindowType() == LANDING)
		{
			if(RenderWindowBody(nuklearContext, nullptr)) return false;
		}//End else if

		//If THIS is reached, I've messed up
		else
		{
			return false;
		}//End else
		
		return true;
	}//End RenderWindow

public:
	NuklearWindowManager(shared_ptr<DataManager> dataManager) : dataManager_(std::move(dataManager))
	{
		//Create landing window
		activeNuklearWindows_.push_back(new NuklearWindow(LANDING, nullptr));

		//Create template window
		activeNuklearWindows_.push_back(new NuklearWindow(TEMPLATE_WINDOW, make_shared<Template>(Template())));

		//Create member window
		activeNuklearWindows_.push_back(new NuklearWindow(MEMBER_WINDOW, make_shared<Member>(Member())));
	}//End constructor

	bool RenderAllActiveWindows(nk_context* nuklearContext)
	{
		for (NuklearWindow* window : activeNuklearWindows_)
		{
			if(const bool success = RenderWindow(nuklearContext, window); !success) return false;
		}//End for
		return true;
	}//End RenderAllActiveWindows
	
	bool CreateNewWindow(const char* windowTitle)
	{
		return true;
	}//End CreateNewWindow
};
#endif