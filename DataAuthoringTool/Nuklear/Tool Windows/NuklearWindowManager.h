#pragma once
//Header-only class to take advantage of nuklear.h being declared earlier in the program execution, meaning this class has access to its functions
//Not ideal, but avoids re-definition errors with Nuklear that would otherwise crop up
#ifndef NUKLEAR_WINDOW_MANAGER
#define NUKLEAR_WINDOW_MANAGER
#include "NuklearWindow.h"
#include <vector>

#include "../../Data/Shared/DataInterfaces.h"
#include "../../Data/Shared/DataTypes.h"
#include "../../Data/Members/Member.h"
#include "../../Data/DataManager.h"
#include "../../Data/Groups/Group.h"
#include "../../Data/Members/MemberField.h"
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
	static DataType data_;

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
			for (int i = 0; i < memberData->GetNumberOfFields(); i++)
			{
				MemberField field = memberData->GetFieldAtIndex(i);

				nk_layout_row_dynamic(nuklearContext, 24, 1);
					//Name of field label followed by type
					nk_label(nuklearContext, field.GetNameAndTypeLabel(), NK_LEFT);

				nk_layout_row_dynamic(nuklearContext, 24, 3);
					//Input field for data
					switch(field.GetDataType())
					{
						case DataType::NONE:
							nk_label(nuklearContext, "ERROR: This field does not have a type! Please go to the template and choose a data type for this field.", NK_LEFT);
							break;

						case DataType::STRING:
							//TODO: Add functionality for applying length limit validation rule to max length
							nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field.GetDataBuffer(), field.GetDataBufferCurrentSize(), *field.GetDataBufferMaxSize(), nk_filter_default);
							break;

						case DataType::INTEGER:
							nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field.GetDataBuffer(), field.GetDataBufferCurrentSize(), *field.GetDataBufferMaxSize(), nk_filter_decimal);
							break;

						case DataType::FLOAT:
							nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field.GetDataBuffer(), field.GetDataBufferCurrentSize(), *field.GetDataBufferMaxSize(), nk_filter_float);
							break;

						case DataType::CHAR:
							nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field.GetDataBuffer(), field.GetDataBufferCurrentSize(), 1, nk_filter_default);
							break;

						case DataType::BOOLEAN:
							nk_checkbox_label(nuklearContext, "", reinterpret_cast<nk_bool*>(field.GetBooleanData()));
							break;

						default: 
							nk_label(nuklearContext, "ERROR: This field does not have a type! Please go to the template and choose a data type for this field.", NK_LEFT);
					}//End switch

					if(field.GetDataType() != DataType::BOOLEAN)
					{
						//Delete/Clear button for field
						if(nk_button_label(nuklearContext, "CLEAR FIELD")) field.Delete();

						//Validate button for field
						if(nk_button_label(nuklearContext, "VALIDATE")) field.Validate();
					}//End if
					else
					{
						//Two blank labels to fill out columns
						nk_label(nuklearContext, "", NK_LEFT);
						nk_label(nuklearContext, "", NK_LEFT);
					}//End else
			}//End for
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
			for (int i = 0; i < templateData->GetNumberOfFields(); i++)
			{
				//Field Name
				nk_layout_row_dynamic(nuklearContext, 24, 4);
					nk_label(nuklearContext, "Field Name", NK_LEFT);
					nk_label(nuklearContext, "Type", NK_LEFT);
					nk_label(nuklearContext, "Validation Rules", NK_LEFT);
					nk_label(nuklearContext, "", NK_LEFT);

					TemplateField field = templateData->GetFieldAtIndex(i);
					//Field Name
					nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field.GetIDBuffer(), field.GetIDBufferCurrentLength(), TemplateField::GetBufferMax(), nk_filter_ascii);
					//Field Type
					nk_combo(nuklearContext, DataType::typeLabels_, DataType::typeLabelsCount, field.GetDataType(), 24, nk_vec2(300, 300));
					//Validation Rules

					//Delete Field Button
					if(nk_button_label(nuklearContext, "DELETE FIELD")) field.Delete();
			}//End for

			//Add Field Button
			if(nk_button_label(nuklearContext, "ADD FIELD")) templateData->AddNewField();
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