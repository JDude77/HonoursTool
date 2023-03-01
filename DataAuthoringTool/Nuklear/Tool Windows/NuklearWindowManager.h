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

class NuklearWindowManager
{
private:
	static vector<shared_ptr<Template>> cachedTemplates_;
	shared_ptr<DataManager> dataManager_;
	vector<NuklearWindow*> activeNuklearWindows_;

	void RenderTemplateHeaderDropdown(nk_context* nuklearContext, shared_ptr<Member> memberData)
	{
		//Update cached templates
		//TODO: Refactor this to not happen every frame of rendering a member window - only on templates updating
		cachedTemplates_ = dataManager_->GetAllInstancesOfType(memberData->GetType());

		vector<char*> templateNames;
		for (const auto& cachedTemplate : cachedTemplates_)
		{
			templateNames.push_back(cachedTemplate->GetNameBuffer());
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

		nk_layout_row_dynamic(nuklearContext, 24, 1);
			//If the data we're handling is a member
			if(auto memberData = std::dynamic_pointer_cast<Member>(windowData))
			{
				RenderTemplateHeaderDropdown(nuklearContext, memberData);
			}//End if

			//If the data we're handling is a group
			else if(const auto groupData = std::dynamic_pointer_cast<Group>(windowData))
			{
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
				if(!RenderWindowHeader(nuklearContext, windowData)) return false;

				//Render Body

				//Render Footer
				if(!RenderWindowFooter(nuklearContext, windowData)) return false;
			}//End if
			nk_end(nuklearContext);
		}//End if
		else
		{
			return false;
		}//End else
		
		return true;
	}//End RenderWindow

public:
	NuklearWindowManager(shared_ptr<DataManager> dataManager);
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