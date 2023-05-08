#pragma once
//Header-only class to take advantage of nuklear.h being declared earlier in the program execution, meaning this class has access to its functions
//Not ideal, but avoids re-definition errors with Nuklear that would otherwise crop up
#ifndef NUKLEAR_WINDOW_MANAGER
#define NUKLEAR_WINDOW_MANAGER
#include <iostream>

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

enum struct BUTTON_STYLE
{
	OPEN_TEMPLATE_WINDOW_BUTTON,
	OPEN_MEMBER_WINDOW_BUTTON,
	OPEN_GROUP_WINDOW_BUTTON,
	VALIDATE_BUTTON,
	DELETE_BUTTON,
	ADD_BUTTON,
	EXPORT_BUTTON
};

enum struct TEMPLATE_REFRESH
{
	FIELD_NAME_CHANGE = 1,
	FIELD_TYPE_CHANGE = 2,
	FIELD_ADD = 4,
	FIELD_REMOVE = 8
};

class NuklearWindowManager
{
private:
	inline static shared_ptr<vector<shared_ptr<Template>>> cachedTemplates_;

	string lastWindowBeforeRefresh_;
	shared_ptr<DataManager> dataManager_;
	vector<NuklearWindow*> activeNuklearWindows_;
	map<BUTTON_STYLE, nk_style_button> buttonStyleMap_;
	nk_style_combo addComboStyle_{};
	nk_style_combo defaultComboStyle_{};

	shared_ptr<float> windowWidth_ = make_shared<float>(0.0f);
	shared_ptr<float> windowHeight_ = make_shared<float>(0.0f);

	//Initialisation
	void CreateStyles(const nk_context* nuklearContext)
	{
		//Open Template Window style
		nk_style_button openTemplateWindowStyle = nuklearContext->style.button;
		openTemplateWindowStyle.normal = nk_style_item_color(nk_rgb(40, 115, 115));
		openTemplateWindowStyle.hover = nk_style_item_color(nk_rgb(70, 145, 145));
		openTemplateWindowStyle.active = nk_style_item_color(nk_rgb(30, 105, 105));
		openTemplateWindowStyle.border_color = nk_rgb(250, 255, 255);
		openTemplateWindowStyle.text_normal = nk_rgb(250, 250, 250);
		openTemplateWindowStyle.text_hover = nk_rgb(253, 253, 253);
		openTemplateWindowStyle.text_active = nk_rgb(245, 245, 245);
		buttonStyleMap_.emplace(BUTTON_STYLE::OPEN_TEMPLATE_WINDOW_BUTTON, openTemplateWindowStyle);

		//Open Member Window style
		nk_style_button openMemberWindowStyle = nuklearContext->style.button;
		openMemberWindowStyle.normal = nk_style_item_color(nk_rgb(115, 40, 115));
		openMemberWindowStyle.hover = nk_style_item_color(nk_rgb(145, 70, 145));
		openMemberWindowStyle.active = nk_style_item_color(nk_rgb(105, 30, 105));
		openMemberWindowStyle.border_color = nk_rgb(255, 250, 255);
		openMemberWindowStyle.text_normal = nk_rgb(250, 250, 250);
		openMemberWindowStyle.text_hover = nk_rgb(253, 253, 253);
		openMemberWindowStyle.text_active = nk_rgb(245, 245, 245);
		buttonStyleMap_.emplace(BUTTON_STYLE::OPEN_MEMBER_WINDOW_BUTTON, openMemberWindowStyle);

		//Open Group Window style
		nk_style_button openGroupWindowStyle = nuklearContext->style.button;
		openGroupWindowStyle.normal = nk_style_item_color(nk_rgb(115, 115, 40));
		openGroupWindowStyle.hover = nk_style_item_color(nk_rgb(145, 145, 70));
		openGroupWindowStyle.active = nk_style_item_color(nk_rgb(105, 105, 30));
		openGroupWindowStyle.border_color = nk_rgb(255, 255, 250);
		openGroupWindowStyle.text_normal = nk_rgb(250, 250, 250);
		openGroupWindowStyle.text_hover = nk_rgb(253, 253, 253);
		openGroupWindowStyle.text_active = nk_rgb(245, 245, 245);
		buttonStyleMap_.emplace(BUTTON_STYLE::OPEN_GROUP_WINDOW_BUTTON, openGroupWindowStyle);

		//Validate style
		nk_style_button validateStyle = nuklearContext->style.button;
		validateStyle.normal = nk_style_item_color(nk_rgb(180, 55, 225));
		validateStyle.hover = nk_style_item_color(nk_rgb(210, 85, 255));
		validateStyle.active = nk_style_item_color(nk_rgb(170, 45, 215));
		validateStyle.border_color = nk_rgb(255, 250, 255);
		validateStyle.text_normal = nk_rgb(250, 250, 250);
		validateStyle.text_hover = nk_rgb(253, 253, 253);
		validateStyle.text_active = nk_rgb(245, 245, 245);
		buttonStyleMap_.emplace(BUTTON_STYLE::VALIDATE_BUTTON, validateStyle);

		//Delete style
		nk_style_button deleteStyle = nuklearContext->style.button;
		deleteStyle.normal = nk_style_item_color(nk_rgb(225, 30, 30));
		deleteStyle.hover = nk_style_item_color(nk_rgb(255, 60, 60));
		deleteStyle.active = nk_style_item_color(nk_rgb(215, 20, 20));
		deleteStyle.border_color = nk_rgb(255, 250, 250);
		deleteStyle.text_normal = nk_rgb(250, 250, 250);
		deleteStyle.text_hover = nk_rgb(253, 253, 253);
		deleteStyle.text_active = nk_rgb(245, 245, 245);
		buttonStyleMap_.emplace(BUTTON_STYLE::DELETE_BUTTON, deleteStyle);

		//Export style
		nk_style_button exportStyle = nuklearContext->style.button;
		exportStyle.normal = nk_style_item_color(nk_rgb(210, 115, 60));
		exportStyle.hover = nk_style_item_color(nk_rgb(240, 145, 90));
		exportStyle.active = nk_style_item_color(nk_rgb(200, 105, 50));
		exportStyle.border_color = nk_rgb(255, 255, 250);
		exportStyle.text_normal = nk_rgb(250, 250, 250);
		exportStyle.text_hover = nk_rgb(253, 253, 253);
		exportStyle.text_active = nk_rgb(245, 245, 245);
		buttonStyleMap_.emplace(BUTTON_STYLE::EXPORT_BUTTON, exportStyle);

		//Add style (button)
		nk_style_button addButtonStyle = nuklearContext->style.button;
		addButtonStyle.normal = nk_style_item_color(nk_rgb(60, 210, 90));
		addButtonStyle.hover = nk_style_item_color(nk_rgb(90, 240, 120));
		addButtonStyle.active = nk_style_item_color(nk_rgb(50, 200, 80));
		addButtonStyle.border_color = nk_rgb(250, 255, 250);
		addButtonStyle.text_normal = nk_rgb(250, 250, 250);
		addButtonStyle.text_hover = nk_rgb(253, 253, 253);
		addButtonStyle.text_active = nk_rgb(245, 245, 245);
		buttonStyleMap_.emplace(BUTTON_STYLE::ADD_BUTTON, addButtonStyle);


		//Add style (combo)
		addComboStyle_ = nuklearContext->style.combo;
		addComboStyle_.normal = nk_style_item_color(nk_rgb(60, 210, 90));
		addComboStyle_.hover = nk_style_item_color(nk_rgb(90, 240, 120));
		addComboStyle_.active = nk_style_item_color(nk_rgb(50, 200, 80));
		addComboStyle_.border_color = nk_rgb(250, 255, 250);
		addComboStyle_.label_normal = nk_rgb(250, 250, 250);
		addComboStyle_.label_hover = nk_rgb(253, 253, 253);
		addComboStyle_.label_active = nk_rgb(245, 245, 245);
		addComboStyle_.button = addButtonStyle;

		//Set default style for switching back
		defaultComboStyle_ = nuklearContext->style.combo;
	}//End CreateButtonStyles

	//Data Clean-Up
	static void PreventNegativeAndNonNegativeRule(TemplateField* field, const int index)
	{
		//Check to see if the number is negative rule has just been activated
		if (field->GetValidationRules()->at(index).first == NUMBER_IS_NEGATIVE)
		{
			//If the number is not negative rule is ALSO active
			if (field->GetValidationRules()->at(index - 1).second == 1 && field->GetValidationRules()->at(index).second == 1)
			{
				//Deactivate the number is not negative rule
				field->GetValidationRules()->at(index - 1).second = 0;
			}//End if
		}//End if

		//Check to see if the number is not negative rule has just been activated
		if (field->GetValidationRules()->at(index).first == NUMBER_IS_NOT_NEGATIVE)
		{
			//If the number is negative rule is ALSO active
			if (field->GetValidationRules()->at(index + 1).second == 1 && field->GetValidationRules()->at(index).second == 1)
			{
				//Deactivate the number is negative rule
				field->GetValidationRules()->at(index + 1).second = 0;
			}//End if
		}//End if
	}//End PreventNegativeAndNonNegativeRule
	static void ClearDeletedCharacters(char* textBuffer, const int bufferCurrentLength)
	{
		//Check to see if the current buffer has more characters in it than there should be
		if (strlen(textBuffer) > bufferCurrentLength)
		{
			//Loop through every value after the end of the expected length of the buffer
			for (int j = bufferCurrentLength; j < TemplateField::GetBufferMax(); j++)
			{
				//Replace with a null character if it isn't already null - if it is, stop
				if (textBuffer[j] == '\0') break;
				textBuffer[j] = '\0';
			}//End for
		}//End if
	}//End ClearDeletedCharacters
	bool DeleteEmptyDataEntriesFromDataManager(nk_context* nuklearContext, NuklearWindow* nuklearWindow, const shared_ptr<PrimaryData>& windowData, const string& id, bool& value)
	{
		//After data processing, check if the window is closed
		if (nk_window_is_closed(nuklearContext, id.c_str()) || nk_window_is_hidden(nuklearContext, id.c_str()))
		{
			//Check if the window's data is a valid member
			if (const auto memberData = dynamic_pointer_cast<Member>(windowData); memberData != nullptr)
			{
				//If the data is empty
				if (memberData->IsEmpty())
				{
					//Remove the empty item from the data manager to not clog up the system with blank members
					dataManager_->RemoveInstanceFromDataMap(*memberData);
				}//End if
				//Remove the member window from the active window list
				activeNuklearWindows_.erase(std::ranges::find(activeNuklearWindows_, nuklearWindow));
				nk_end(nuklearContext);
				value = false;
				return true;
			}//End if

			//Check if the window's data is a valid template
			if (const auto templateData = dynamic_pointer_cast<Template>(windowData); templateData != nullptr)
			{
				//If the data is empty
				if (templateData->IsEmpty())
				{
					//Remove the empty item from the data manager to not clog up the system with blank templates
					dataManager_->RemoveInstanceFromDataMap(*templateData);
				}//End if
				//Remove the template window from the active window list
				activeNuklearWindows_.erase(std::ranges::find(activeNuklearWindows_, nuklearWindow));
				nk_end(nuklearContext);
				value = false;
				return true;
			}//End if

			//Check if the window's data is a valid template
			if (const auto groupData = dynamic_pointer_cast<Group>(windowData); groupData != nullptr)
			{
				//If the data is empty
				if (groupData->IsEmpty())
				{
					//Remove the empty item from the data manager to not clog up the system with blank templates
					dataManager_->RemoveInstanceFromDataMap(*groupData);
				}//End if
				//Remove the template window from the active window list
				activeNuklearWindows_.erase(std::ranges::find(activeNuklearWindows_, nuklearWindow));
				nk_end(nuklearContext);
				value = false;
				return true;
			}//End if
		}//End if
		return false;
	}//End DeleteEmptyDataEntriesFromDataManager

	//Custom Header Elements
	void RenderTemplateHeaderDropdown(nk_context* nuklearContext, const shared_ptr<Member>& memberData) const
	{
		//Update cached templates
		cachedTemplates_ = make_shared<vector<shared_ptr<Template>>>(dataManager_->GetAllTemplates());

		const int cachedTemplateIndex = memberData->GetTemplateIndex();

		vector<const char*> templateNames;
		for (auto& temp : *cachedTemplates_)
		{
			templateNames.push_back(temp->GetNameBuffer());
		}//End for

		//Render templates dropdown
		memberData->SetTemplateIndex(nk_combo(nuklearContext, templateNames.data(), templateNames.size(), memberData->GetTemplateIndex(), 24, nk_vec2(300, 300)));

		//Set template depending on index
		if (cachedTemplateIndex != memberData->GetTemplateIndex()) memberData->SetType(cachedTemplates_->at(memberData->GetTemplateIndex()));
	}//End RenderTemplateHeaderDropdown
	bool RenderGroupTemplateTabs(nk_context* nuklearContext, const shared_ptr<Group>& groupData, int& activeTab, bool& value) const
	{
		cachedTemplates_ = make_shared<vector<shared_ptr<Template>>>(dataManager_->GetAllTemplates());

		//Get the number of tabs that should be active
		const int numberOfTabs = groupData->GetNumberOfTemplates();

		//Get list of internal IDs and names for templates
		map<const int, const char*> templates;
		const vector<shared_ptr<Template>> groupDataExistingTemplates = groupData->GetTemplates();
		for (const shared_ptr<Template>& temp : *cachedTemplates_)
		{
			//Don't gather names of empty templates or the dedicated None template
			if (temp->GetInternalID() == -1 || temp->IsEmpty()) continue;

			//Also don't show any templates already in the group
			if (std::ranges::find(groupDataExistingTemplates, temp) != groupDataExistingTemplates.end()) continue;

			templates.emplace(temp->GetInternalID(), temp->GetNameBuffer());
		}//End for

		//Set up the tab styling
		nk_style_push_vec2(nuklearContext, &nuklearContext->style.window.spacing, nk_vec2(0, 0));
		nk_style_push_float(nuklearContext, &nuklearContext->style.button.rounding, 0);
		nk_layout_row_dynamic(nuklearContext, 24, numberOfTabs + 1);

		//Tab with template which has added members of that type below it
		for (int i = 0; i < groupData->GetNumberOfTemplates(); i++)
		{
			//Clickable tabs to choose which template is being worked with
			if (const char* templateLabel = groupData->GetTemplates().at(i)->GetIDBuffer(); nk_selectable_tab(nuklearContext, templateLabel, activeTab == i))
			{
				activeTab = i;
				groupData->SetActiveTemplateTab(i);
			}//End if
		}//End for

		//Only show the ability to add templates if there are templates to add
		if (!templates.empty())
		{
			nuklearContext->style.combo = addComboStyle_;
			//Dropdown/button combo which selects and adds template at the same time
			if (nk_combo_begin_label(nuklearContext, "Add Template To Group", nk_vec2(600, 400)))
			{
				//Create a vector of template names already in the group
				vector<const char*> groupTemplateNames;
				for (const auto& temp : groupData->GetTemplates())
				{
					groupTemplateNames.push_back(temp->GetIDBuffer());
				}//End for

				nk_layout_row_dynamic(nuklearContext, 24, 1);

				//Render every button of templates to add
				int templateNumber = 0;
				for(auto iterator = templates.begin(); iterator != templates.end(); ++iterator)
				{
					//Show a button to allow for adding it to the group
					if (nk_button_label(nuklearContext, iterator->second))
					{
						//groupData->SetActiveTemplateTab(templateNumber);
						groupData->FlagTemplateToAdd(iterator->first);
						break;
					}//End if
					templateNumber++;
				}//End for

				if (groupData->GetAddNewTemplateFlag())
				{
					groupData->AddTemplateToGroup(dataManager_->FindTemplateByInternalID(groupData->GetFlaggedTemplateInternalID()));
					groupData->SetActiveTemplateTab(groupData->GetNumberOfTemplates() - 1);
					nk_combo_close(nuklearContext);
					nk_style_pop_float(nuklearContext);
					nk_style_pop_vec2(nuklearContext);
					nk_combo_end(nuklearContext);
					value = true;
					return true;
				}//End if

				nk_combo_end(nuklearContext);
			}//End if
		}//End if
		else
		{
			nk_label(nuklearContext, "NO TEMPLATES TO ADD TO GROUP", NK_TEXT_CENTERED);
		}//End else
		nuklearContext->style.combo = defaultComboStyle_;
		nk_style_pop_float(nuklearContext);
		nk_style_pop_vec2(nuklearContext);

		return false;
	}//End RenderGroupTemplateTabs

	//Window Render Types
	void RenderLandingBody(nk_context* nuklearContext)
	{
		//Template Button
		nk_layout_space_begin(nuklearContext, NK_DYNAMIC, *windowHeight_ / 6.0f, 3);
		nk_layout_space_push(nuklearContext, nk_rect(0.01f, 0.1f, 0.45f, 0.45f));
		if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::OPEN_TEMPLATE_WINDOW_BUTTON], "Open New Template Window"))
			CreateNewWindow("Template Window", WINDOW_TYPE::TEMPLATE_WINDOW);
		nk_layout_space_push(nuklearContext, nk_rect(0.475f, 0.05f, 0.55f, 0.25f));
		nk_label(nuklearContext, "Templates", NK_TEXT_LEFT);
		nk_layout_space_push(nuklearContext, nk_rect(0.475f, 0.275f, 0.55f, 0.45f));
		nk_label_wrap(nuklearContext, "The blueprints you can use to define Member types, as well as set up validation rules.");
		nk_layout_space_end(nuklearContext);

		//Member Button
		nk_layout_space_begin(nuklearContext, NK_DYNAMIC, *windowHeight_ / 6.0f, 3);
		nk_layout_space_push(nuklearContext, nk_rect(0.01f, 0.1f, 0.45f, 0.45f));
		if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::OPEN_MEMBER_WINDOW_BUTTON], "Open New Member Window"))
			CreateNewWindow("Member Window", WINDOW_TYPE::MEMBER_WINDOW);
		nk_layout_space_push(nuklearContext, nk_rect(0.475f, 0.05f, 0.55f, 0.25f));
		nk_label(nuklearContext, "Members", NK_TEXT_LEFT);
		nk_layout_space_push(nuklearContext, nk_rect(0.475f, 0.275f, 0.55f, 0.45f));
		nk_label_wrap(nuklearContext, "The objects you can create, using a Template as the guide for how everything is laid out and validated.");
		nk_layout_space_end(nuklearContext);

		//Group Button
		nk_layout_space_begin(nuklearContext, NK_DYNAMIC, *windowHeight_ / 6.0f, 3);
		nk_layout_space_push(nuklearContext, nk_rect(0.01f, 0.1f, 0.45f, 0.45f));
		if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::OPEN_GROUP_WINDOW_BUTTON], "Open New Group Window"))
			CreateNewWindow("Group Window", WINDOW_TYPE::GROUP_WINDOW);
		nk_layout_space_push(nuklearContext, nk_rect(0.475f, 0.05f, 0.55f, 0.25f));
		nk_label(nuklearContext, "Groups", NK_TEXT_LEFT);
		nk_layout_space_push(nuklearContext, nk_rect(0.475f, 0.275f, 0.55f, 0.45f));
		nk_label_wrap(nuklearContext, "Allows you to create collections of Members and Templates, for quick validation and export.");
		nk_layout_space_end(nuklearContext);

		//List Of All Data
		nk_layout_space_begin(nuklearContext, NK_DYNAMIC, *windowHeight_ / 2.75f, INT_MAX);

		//List Of All Templates
		nk_layout_space_push(nuklearContext, nk_rect(0.01f, 0.0f, 0.3f, 1.0f));
		if(nk_group_begin(nuklearContext, "All Templates", NK_WINDOW_TITLE | NK_WINDOW_BORDER))
		{
			nk_layout_row_dynamic(nuklearContext, 24, 1);
			for (const auto templates = dataManager_->GetAllTemplates(); const auto& templateData : templates)
			{
				//Don't allow the user to open the "None" template placeholder
				if(templateData->GetInternalID() == -1) continue;

				//Button for each other Template
				if(nk_button_label(nuklearContext, templateData->GetIDBuffer()))
					CreateNewWindow("Template Window", WINDOW_TYPE::TEMPLATE_WINDOW, templateData, nuklearContext);
			}//End for
			nk_group_end(nuklearContext);
		}//End if

		//List Of All Members
		nk_layout_space_push(nuklearContext, nk_rect(0.333f, 0.0f, 0.3f, 1.0f));
		if(nk_group_begin(nuklearContext, "All Members", NK_WINDOW_TITLE | NK_WINDOW_BORDER))
		{
			nk_layout_row_dynamic(nuklearContext, 24, 1);
			for (const auto members = dataManager_->GetAllMembers(); const auto& memberData : members)
			{
				//Button for each other Member
				if(nk_button_label(nuklearContext, memberData->GetIDBuffer()))
					CreateNewWindow("Member Window", WINDOW_TYPE::MEMBER_WINDOW, memberData, nuklearContext);
			}//End for
			nk_group_end(nuklearContext);
		}//End if

		//List Of All Groups
		nk_layout_space_push(nuklearContext, nk_rect(0.667f, 0.0f, 0.3f, 1.0f));
		if(nk_group_begin(nuklearContext, "All Groups", NK_WINDOW_TITLE | NK_WINDOW_BORDER))
		{
			nk_layout_row_dynamic(nuklearContext, 24, 1);
			for (const auto groups = dataManager_->GetAllGroups(); const auto& groupData : groups)
			{
				//Button for each other Member
				if(nk_button_label(nuklearContext, groupData->GetIDBuffer()))
					CreateNewWindow("Group Window", WINDOW_TYPE::GROUP_WINDOW, groupData, nuklearContext);
			}//End for
			nk_group_end(nuklearContext);
		}//End if

		nk_layout_space_end(nuklearContext);
	}//End RenderLandingBody
	void RenderMemberBody(nk_context* nuklearContext, const shared_ptr<Member>& memberData, const NuklearWindow* nuklearWindow)
	{
		for (int i = 0; i < memberData->GetNumberOfFields(); i++)
		{
			MemberField* field = memberData->GetFieldAtIndex(i);

			nk_layout_row_dynamic(nuklearContext, 24, 1);
			//Name of field label followed by type
			nk_label(nuklearContext, field->GetNameAndTypeLabel(), NK_LEFT);

			//Input field for data
			switch (field->GetDataType())
			{
			case DataType::DATA_TYPE::STRING:
				nk_layout_row_dynamic(nuklearContext, 24, 3);
				nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field->GetDataBuffer(), field->GetDataBufferCurrentSize(), *field->GetDataBufferMaxSize(), nk_filter_default);
				break;

			case DataType::DATA_TYPE::INTEGER:
				nk_layout_row_dynamic(nuklearContext, 24, 3);
				nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field->GetDataBuffer(), field->GetDataBufferCurrentSize(), *field->GetDataBufferMaxSize(), nk_filter_decimal);
				break;

			case DataType::DATA_TYPE::FLOAT:
				nk_layout_row_dynamic(nuklearContext, 24, 3);
				nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field->GetDataBuffer(), field->GetDataBufferCurrentSize(), *field->GetDataBufferMaxSize(), nk_filter_float);
				break;

			case DataType::DATA_TYPE::CHAR:
				nk_layout_row_dynamic(nuklearContext, 24, 3);
				nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field->GetDataBuffer(), field->GetDataBufferCurrentSize(), 2, nk_filter_default);
				break;

			case DataType::DATA_TYPE::BOOLEAN:
				{
					nk_layout_row_dynamic(nuklearContext, 24, 1);
					const char* label = *field->GetBooleanData() ? "TRUE" : "FALSE";
					nk_checkbox_label(nuklearContext, label, reinterpret_cast<nk_bool*>(field->GetBooleanData()));
				}//End Boolean data handling
				break;

			case DataType::DATA_TYPE::NONE:
				{
					nk_layout_row_dynamic(nuklearContext, 32, 1);
					nk_label_wrap(nuklearContext, "ERROR: This field does not have a type! Please go to the template and choose a data type for this field.");
				}//End None hadnling
			}//End switch
			
			if (field->GetDataType() != DataType::DATA_TYPE::BOOLEAN && field->GetDataType() != DataType::DATA_TYPE::NONE)
			{
				ClearDeletedCharacters(field->GetDataBuffer(), *field->GetDataBufferCurrentSize());

				//Delete/Clear button for field
				if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::DELETE_BUTTON], "CLEAR FIELD")) field->Delete();

				//Validate button for field
				if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::VALIDATE_BUTTON], "VALIDATE")) 
				{
					nuklearWindow->ClearSubFooterText();
					field->Validate(nuklearWindow->GetSubFooterText());
				}//End if
			}//End if
		}//End for
		dataManager_->UpdateInstance(memberData);
	}
	bool RenderGroupBody(nk_context* nuklearContext, const shared_ptr<Group>& groupData, const NuklearWindow* nuklearWindow, bool& value)
	{
		int activeTab = *groupData->GetActiveTemplateTab();

		//Render the tabs to let you select which template is currently active
		//Needs a formatting update but good enough for now
		if (bool tabsUpdated; RenderGroupTemplateTabs(nuklearContext, groupData, activeTab, tabsUpdated))
		{
			value = tabsUpdated;
			return true;
		}
		//Show all members in the group
		const auto groupMembers = groupData->GetMembers();
		vector<shared_ptr<Member>> groupMembersOfTemplateTab;
		for (auto& member : groupMembers)
		{
			if (strcmp(member->GetType()->GetIDBuffer(), groupData->GetTemplates().at(activeTab)->GetIDBuffer()) == 0)
			{
				groupMembersOfTemplateTab.push_back(member);
			}//End if
		}//End for

		//Set formatting for name - validate - delete row layout
		nk_layout_row_dynamic(nuklearContext, 24, 3);
		for (const auto& member : groupMembersOfTemplateTab)
		{
			string fullNameOfMember;
			fullNameOfMember.append(member->GetNameBuffer());
			fullNameOfMember.append(" (ID: ");
			fullNameOfMember.append(member->GetIDBuffer());
			fullNameOfMember.append(")");

			nk_label(nuklearContext, fullNameOfMember.c_str(), NK_TEXT_LEFT);

			if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::VALIDATE_BUTTON], "VALIDATE"))
			{
				nuklearWindow->ClearSubFooterText();
				member->Validate(nuklearWindow->GetSubFooterText());
			}//End if
			if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::DELETE_BUTTON], "REMOVE"))
			{
				groupData->RemoveMemberFromGroup(member);
			}//End if
		}//End for

		//Add member button
		if (!groupData->GetTemplates().empty())
		{
			nk_layout_row_dynamic(nuklearContext, 24, 1);
			static vector<shared_ptr<Member>> addableMembers;
			for (shared_ptr<Member>& member : dataManager_->GetAllMembers())
			{
				if (std::ranges::find(addableMembers, member) != addableMembers.end()) continue;
				addableMembers.push_back(member);
			}//End for

			//Cache all members from the data manager for sorting
			if (!addableMembers.empty())
			{
				nuklearContext->style.combo = addComboStyle_;
				//Similar button/dropdown system to the add template dropdown
				if (nk_combo_begin_label(nuklearContext, "ADD MEMBER", nk_vec2(300, 300)))
				{
					//Create the vector of sorted members
					vector<shared_ptr<Member>> allMembersOfTemplateNotAlreadyInGroup;
					//For every member
					for (auto& addableMember : addableMembers)
					{
						//If it doesn't have data, don't waste time on it
						if (addableMember->IsEmpty() || addableMember->GetType() == nullptr) continue;

						//If the member is of the same type as the current active template tab
						if (strcmp(addableMember->GetType()->GetIDBuffer(), groupData->GetTemplates().at(activeTab)->GetIDBuffer()) == 0)
						{
							//If the member isn't already in the group
							if (std::ranges::find(groupMembersOfTemplateTab, addableMember) == groupMembersOfTemplateTab.end())
							{
								//Add it to the list of possible members to add
								allMembersOfTemplateNotAlreadyInGroup.push_back(addableMember);
							}//End if
						}//End if
					}//End for
					nk_layout_row_dynamic(nuklearContext, 24, 1);

					//Add a button for each addable member
					for (const auto& member : allMembersOfTemplateNotAlreadyInGroup)
					{
						string fullNameOfMember;
						fullNameOfMember.append(member->GetNameBuffer());
						fullNameOfMember.append(" (ID: ");
						fullNameOfMember.append(member->GetIDBuffer());
						fullNameOfMember.append(")");

						//Button in the dropdown to allow adding the member to the group

						if (nk_button_label(nuklearContext, fullNameOfMember.c_str()))
						{
							groupData->AddMemberToGroup(member);
							//Force refresh of addable members of type
							nk_combo_close(nuklearContext);
							nk_combo_end(nuklearContext);
							allMembersOfTemplateNotAlreadyInGroup.erase(allMembersOfTemplateNotAlreadyInGroup.begin(), allMembersOfTemplateNotAlreadyInGroup.end());
							addableMembers.erase(addableMembers.begin(), addableMembers.end());
							value = true;
							return true;
						}//End if
					}//End for
					nk_combo_end(nuklearContext);
				}//End if
				nuklearContext->style.combo = defaultComboStyle_;
			}//End if
		}//End if

		dataManager_->UpdateInstance(groupData);
		return false;
	}
	void RenderTemplateBody(nk_context* nuklearContext, const shared_ptr<Template>& templateData)
	{
		int_least8_t refresh = 0;
		int refreshIndex = -1;

		//For Each Field
		for (int i = 0; i < templateData->GetNumberOfFields(); i++)
		{
			//UI Labels
			nk_layout_row_dynamic(nuklearContext, 24, 4);
			nk_label(nuklearContext, "Field Name", NK_LEFT);
			nk_label(nuklearContext, "Type", NK_LEFT);
			nk_label(nuklearContext, "Validation Rules", NK_LEFT);
			nk_label(nuklearContext, "", NK_LEFT);

			TemplateField* field = templateData->GetFieldAtIndex(i);
			string cachedName = field->GetIDBuffer();
			//Field Name
			nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field->GetIDBuffer(), field->GetIDBufferCurrentLength(), TemplateField::GetBufferMax(), nk_filter_ascii);
			//Clear deleted characters
			ClearDeletedCharacters(field->GetIDBuffer(), *field->GetIDBufferCurrentLength());
			if(~refresh & static_cast<int>(TEMPLATE_REFRESH::FIELD_NAME_CHANGE) && strcmp(cachedName.c_str(), field->GetIDBuffer()) != 0)
			{
				refresh += static_cast<int>(TEMPLATE_REFRESH::FIELD_NAME_CHANGE);
				refreshIndex = i;
			}//End if

			//Field Type
			const DataType::DATA_TYPE cachedDataType = *field->GetDataType();
			field->SetDataType(nk_combo(nuklearContext, DataType::typeLabels_, std::size(DataType::typeLabels_), *field->GetDataTypeAsInt(), 24, nk_vec2(300, 300)));
			//If the template has changed, refresh all members of the type
			if(~refresh & static_cast<int>(TEMPLATE_REFRESH::FIELD_TYPE_CHANGE) && cachedDataType != *field->GetDataType())
			{
				refresh += static_cast<int>(TEMPLATE_REFRESH::FIELD_TYPE_CHANGE);
				refreshIndex = i;
			}//End if
			//Validation rules multi-select combo box
			//Boolean type and none type have no validation rules
			if (*field->GetDataType() != DataType::DATA_TYPE::NONE && *field->GetDataType() != DataType::DATA_TYPE::BOOLEAN)
			{
				if (nk_combo_begin_label(nuklearContext, "Click To Choose Validation Rules", nk_vec2(500, 300)))
				{
					vector<std::pair<RULES, int>> cachedRules = *field->GetValidationRules();
					auto labels = ValidationRule::GetValidationRuleLabels(*field->GetDataType());
					nk_layout_row_dynamic(nuklearContext, 24, 1);
					for (int j = 0; j < labels.size(); j++)
					{
						nk_selectable_label(nuklearContext, labels[j].c_str(), NK_TEXT_LEFT, &field->GetValidationRules()->at(j).second);

						PreventNegativeAndNonNegativeRule(field, j);
					}//End for
					nk_combo_end(nuklearContext);
				}//End if
			}//End if
			else
			{
				//Blank label to preserve spacing
				nk_label(nuklearContext, "", NK_LEFT);
			}//End else

			//Delete Field Button
			if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::DELETE_BUTTON], "DELETE FIELD"))
			{
				field->Delete();
				if(~refresh & static_cast<int>(TEMPLATE_REFRESH::FIELD_REMOVE))
				{
					refresh += static_cast<int>(TEMPLATE_REFRESH::FIELD_REMOVE);
				}//End if
			}//End if

			//Validation rule fill-out
			if (nk_tree_push_id(nuklearContext, NK_TREE_TAB, "Validation Details", NK_MAXIMIZED, i))
			{
				for (int j = 0; j < field->GetValidationRules()->size(); j++)
				{
					ValidationRuleParameter& validationRuleParameters = *field->GetValidationRuleParameters();
					if (const auto [rule, active] = field->GetValidationRules()->at(j); active != 0)
					{
						switch (rule)
						{
						case ALL_PRESENCE:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 1);
								nk_label(nuklearContext, "The field is not allowed to be empty.", NK_LEFT);
							}//End all presence
							break;
						case STRING_MAX_LENGTH:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 3);
								nk_label(nuklearContext, "Text must not contain more than ", NK_TEXT_LEFT);
								nk_edit_string(nuklearContext,
								               NK_EDIT_SIMPLE,
								               validationRuleParameters.GetBuffer(rule),
								               validationRuleParameters.GetBufferCurrentLength(rule),
								               ValidationRuleParameter::stringMaxLengthBufferSize_,
								               nk_filter_decimal);
								nk_label(nuklearContext, " characters.", NK_TEXT_LEFT);
							}//End string max length
							break;
						case STRING_MIN_LENGTH:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 3);
								nk_label(nuklearContext, "Text must not contain fewer than ", NK_TEXT_LEFT);
								nk_edit_string(nuklearContext,
								               NK_EDIT_SIMPLE,
								               validationRuleParameters.GetBuffer(rule),
								               validationRuleParameters.GetBufferCurrentLength(rule),
								               ValidationRuleParameter::stringMinLengthBufferSize_,
								               nk_filter_decimal);
								nk_label(nuklearContext, " characters.", NK_TEXT_LEFT);
							}//End string min length
							break;
						case STRING_STARTS_WITH_SUBSTRING:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 2);
								nk_label(nuklearContext, "Text must begin with the following: ", NK_TEXT_LEFT);
								nk_edit_string(nuklearContext,
								               NK_EDIT_SIMPLE,
								               validationRuleParameters.GetBuffer(rule),
								               validationRuleParameters.GetBufferCurrentLength(rule),
								               ValidationRuleParameter::stringStartsWithSubstringBufferSize_,
								               nk_filter_default);
							}//End string starts with substring
							break;
						case STRING_ENDS_WITH_SUBSTRING:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 2);
								nk_label(nuklearContext, "Text must end with the following: ", NK_TEXT_LEFT);
								nk_edit_string(nuklearContext,
								               NK_EDIT_SIMPLE,
								               validationRuleParameters.GetBuffer(rule),
								               validationRuleParameters.GetBufferCurrentLength(rule),
								               ValidationRuleParameter::stringEndsWithSubstringBufferSize_,
								               nk_filter_default);
							}//End string ends with substring
							break;
						case NUMBER_IS_NOT_NEGATIVE:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 1);
								nk_label(nuklearContext, "The number must not be negative.", NK_TEXT_LEFT);
							}//End number is not negative
							break;
						case NUMBER_IS_NEGATIVE:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 1);
								nk_label(nuklearContext, "The number must be negative.", NK_TEXT_LEFT);
							}//End number is negative
							break;
						case NUMBER_IS_NOT_ZERO:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 1);
								nk_label(nuklearContext, "The number must not be zero.", NK_TEXT_LEFT);
							}//End number is not zero
							break;
						case NUMBER_IS_LESS_THAN:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 2);
								nk_label(nuklearContext, "The number must be less than ", NK_TEXT_LEFT);
								nk_edit_string(nuklearContext,
								               NK_EDIT_SIMPLE,
								               validationRuleParameters.GetBuffer(rule),
								               validationRuleParameters.GetBufferCurrentLength(rule),
								               ValidationRuleParameter::numberIsLessThanBufferSize_,
								               nk_filter_float);
							}//End number is less than
							break;
						case NUMBER_IS_GREATER_THAN:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 2);
								nk_label(nuklearContext, "The number must be greater than ", NK_TEXT_LEFT);
								nk_edit_string(nuklearContext,
								               NK_EDIT_SIMPLE,
								               validationRuleParameters.GetBuffer(rule),
								               validationRuleParameters.GetBufferCurrentLength(rule),
								               ValidationRuleParameter::numberIsGreaterThanBufferSize_,
								               nk_filter_float);
							}//End number is greater than
							break;
						case INTEGER_DIVISIBLE_BY_INTEGER:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 3);
								nk_label(nuklearContext, "The integer must divide into the whole number ", NK_TEXT_LEFT);
								nk_edit_string(nuklearContext,
								               NK_EDIT_SIMPLE,
								               validationRuleParameters.GetBuffer(rule),
								               validationRuleParameters.GetBufferCurrentLength(rule),
								               ValidationRuleParameter::integerDivisibleByIntegerBufferSize_,
								               nk_filter_decimal);
								nk_label(nuklearContext, " with no remainder.", NK_TEXT_LEFT);
							}//End integer divisible by integer
							break;
						case CHAR_IS_ONE_OF_CHARACTER_SET:
							{
								nk_layout_row_dynamic(nuklearContext, 24, 2);
								nk_label(nuklearContext, "The character must be one of the following characters: ", NK_TEXT_LEFT);
								nk_edit_string(nuklearContext,
								               NK_EDIT_SIMPLE,
								               validationRuleParameters.GetBuffer(rule),
								               validationRuleParameters.GetBufferCurrentLength(rule),
								               ValidationRuleParameter::charIsOneOfCharacterSetBufferSize_,
								               nk_filter_default);
							}//End char is one of character set
							break;
						case NA: default:;
						}//End switch
						if (validationRuleParameters.GetBuffer(rule) != nullptr)
						{
							ClearDeletedCharacters(validationRuleParameters.GetBuffer(rule), *validationRuleParameters.GetBufferCurrentLength(rule));
						}//End if
					}//End if
				}//End for
				nk_tree_pop(nuklearContext);
			}//End if
		}//End for

		//Add Field Button
		nk_layout_row_dynamic(nuklearContext, 24, 1);
		if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::ADD_BUTTON], "ADD FIELD"))
		{
			templateData->AddNewField();
			if(~refresh & static_cast<int>(TEMPLATE_REFRESH::FIELD_ADD)) refresh += static_cast<int>(TEMPLATE_REFRESH::FIELD_ADD);
		}//End if

		//If the template has changed and there are members of its type, refresh those members
		if(refresh != 0)
		{
			for (const auto members = dataManager_->GetAllMembers(); const auto& member : members)
			{
				if(member->GetType() == nullptr) continue;

				if(refresh & static_cast<int>(TEMPLATE_REFRESH::FIELD_NAME_CHANGE))
				{
					member->RefreshFieldName(refreshIndex);
				}//End if
				else if(refresh & static_cast<int>(TEMPLATE_REFRESH::FIELD_ADD) || refresh & static_cast<int>(TEMPLATE_REFRESH::FIELD_REMOVE))
				{
					member->RefreshFieldQuantity();
				}//End if
				else if(refresh & static_cast<int>(TEMPLATE_REFRESH::FIELD_TYPE_CHANGE))
				{
					member->RefreshFieldType(refreshIndex);
				}//End if
			}//End for
		}//End if

		dataManager_->UpdateInstance(templateData);
	}//End RenderTemplateBody

	//Window Render Sections
	bool RenderWindowHeader(nk_context* nuklearContext, const shared_ptr<PrimaryData>& windowData, const NuklearWindow* nuklearWindow)
	{
		nk_layout_row_dynamic(nuklearContext, 24, 4);
		//Name
		nk_label(nuklearContext, "Name: ", NK_TEXT_LEFT);
		nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, windowData->GetNameBuffer(), windowData->GetNameBufferCurrentLength(), windowData->GetBufferMax(), nk_filter_default);
		windowData->UpdateNameBuffer();

		//ID
		nk_label(nuklearContext, "ID: ", NK_TEXT_LEFT);
		nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, windowData->GetIDBuffer(), windowData->GetIDBufferCurrentLength(), windowData->GetBufferMax(), nk_filter_default);
		windowData->UpdateIDBuffer();
		//If the data we're handling is a member
		if (const auto memberData = dynamic_pointer_cast<Member>(windowData))
		{
			//Member/Template Type dropdown
			nk_layout_row_dynamic(nuklearContext, 24, 1);
			nk_label(nuklearContext, "Member Template/Type", NK_TEXT_LEFT);
			nk_layout_row_dynamic(nuklearContext, 24, 2);
			RenderTemplateHeaderDropdown(nuklearContext, memberData);
			//Validate All Fields button
			if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::VALIDATE_BUTTON], "VALIDATE ALL"))
			{
				nuklearWindow->ClearSubFooterText();
				memberData->Validate(nuklearWindow->GetSubFooterText());
			}//End if
		}//End if

		//If the data we're handling is a group
		else if (const auto groupData = dynamic_pointer_cast<Group>(windowData))
		{
			//Validate All Members button
			nk_layout_row_dynamic(nuklearContext, 24, 1);
			if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::VALIDATE_BUTTON], "VALIDATE"))
			{
				nuklearWindow->ClearSubFooterText();
				groupData->Validate(nuklearWindow->GetSubFooterText());
			}//End if
		}//End else if

		return true;
	}//End RenderWindowHeader
	bool RenderWindowBody(nk_context* nuklearContext, const shared_ptr<PrimaryData>& windowData, const NuklearWindow* currentWindow)
	{
		//LANDING
		if (windowData == nullptr)
		{
			RenderLandingBody(nuklearContext);
		}//End if
		
		//MEMBER
		else if (const auto memberData = dynamic_pointer_cast<Member>(windowData))
		{
			RenderMemberBody(nuklearContext, memberData, currentWindow);
		}//End else if
		
		//GROUP
		else if (const auto groupData = dynamic_pointer_cast<Group>(windowData))
		{
			if (bool value; RenderGroupBody(nuklearContext, groupData, currentWindow, value))
			{
				lastWindowBeforeRefresh_.clear();
				lastWindowBeforeRefresh_.append(currentWindow->GetWindowTitle());
				const auto internalID = std::to_string(groupData->GetInternalID());
				lastWindowBeforeRefresh_.append(internalID);

				return value;
			}//End if
		}//End else if
		
		//TEMPLATE
		else if (const auto templateData = dynamic_pointer_cast<Template>(windowData))
		{
			RenderTemplateBody(nuklearContext, templateData);
		}//End else if

		return true;
	}//End RenderWindowBody
	bool RenderWindowFooter(nk_context* nuklearContext, const shared_ptr<PrimaryData>& windowData, const NuklearWindow* nuklearWindow)
	{
		//Customise label to show what exact type is being exported to lower confusion
		string exportLabel;
		exportLabel.append("EXPORT ");
		if(dynamic_pointer_cast<Member>(windowData)) exportLabel.append("MEMBER");
		else if(dynamic_pointer_cast<Template>(windowData)) exportLabel.append("TEMPLATE");
		else if(dynamic_pointer_cast<Group>(windowData)) exportLabel.append("GROUP");

		nk_layout_row_dynamic(nuklearContext, 24, 1);
		//if (nk_button_label(nuklearContext, "SAVE")) windowData->Save();
		//if (nk_button_label(nuklearContext, "LOAD")) windowData->Load();
		if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::EXPORT_BUTTON], exportLabel.c_str())) 
		{
			nuklearWindow->ClearSubFooterText();
			windowData->Export(nuklearWindow->GetSubFooterText(),windowData.get());
		}//End if
		if (nk_button_label_styled(nuklearContext, &buttonStyleMap_[BUTTON_STYLE::DELETE_BUTTON], "DELETE"))
		{
			dataManager_->DeleteInstanceAndCleanUpConnections(windowData);
			windowData->Delete();
		}//End if

		return true;
	}//End RenderWindowFooter
	bool RenderWindowSubFooter(nk_context* nuklearContext, std::queue<string>* subfooterText) const
	{
		//Subfooter text is currently always going to be the validation or export result
		if(!subfooterText->empty())
		{
			nk_layout_space_begin(nuklearContext, NK_DYNAMIC, *windowHeight_ / 3.0f, 1);
			nk_layout_space_push(nuklearContext, nk_rect(0.0f, 0.0f, 1.0f, 1.0f));
			if(nk_group_begin(nuklearContext, "Output Text", NK_WINDOW_BORDER | NK_WINDOW_TITLE))
			{
				std::queue<string> temp;
				nk_layout_row_dynamic(nuklearContext, 24, 1);
				const int startSize = subfooterText->size();
				for (int i = 0; i < startSize; i++)
				{
					nk_label(nuklearContext, subfooterText->front().c_str(), NK_TEXT_LEFT);
					temp.push(subfooterText->front());
					subfooterText->pop();
				}//End for
				subfooterText->swap(temp);
				nk_group_end(nuklearContext);
			}//End if
			nk_layout_space_end(nuklearContext);
		}//End if

		return true;
	}//End Render Window Sub-Footer

	bool RenderWindow(nk_context* nuklearContext, NuklearWindow* nuklearWindow)
	{
		if (const shared_ptr<PrimaryData> windowData = nuklearWindow->GetWindowData(); windowData != nullptr)
		{
			string id;
			id.append(nuklearWindow->GetWindowTitle());
			id.append(std::to_string(nuklearWindow->GetWindowData()->GetInternalID()));
			if (nk_begin_titled(nuklearContext, id.c_str(), nuklearWindow->GetWindowTitle(), nk_rect(25, 50, 600, 400),
				NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
				NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE))
			{
				//If the window was refreshed (a group window, usually), re-set it to be the focus window
				if(lastWindowBeforeRefresh_ == id)
				{
					nk_window_set_focus(nuklearContext, id.c_str());
					lastWindowBeforeRefresh_.clear();
				}//End if

				//Render Header
				if (nuklearWindow->GetHasHeader())
				{
					if (!RenderWindowHeader(nuklearContext, windowData, nuklearWindow)) return false;
				}//End if

				//Render Body
				if (!RenderWindowBody(nuklearContext, windowData, nuklearWindow)) return false;

				//Render Footer
				if (nuklearWindow->GetHasFooter())
				{
					if (!RenderWindowFooter(nuklearContext, windowData, nuklearWindow)) return false;
				}//End if

				//Render Sub-Footer
				if (nuklearWindow->GetHasSubFooter())
				{
					if (!RenderWindowSubFooter(nuklearContext, nuklearWindow->GetSubFooterText())) return false;
				}//End if
			}//End nk_begin

			if (bool value; DeleteEmptyDataEntriesFromDataManager(nuklearContext, nuklearWindow, windowData, id, value)) return value;

			nk_end(nuklearContext);
		}//End if

		//Landing window has no data, as it is essentially just buttons to open subwindows
		else if (nuklearWindow->GetWindowType() == WINDOW_TYPE::LANDING)
		{
			if (nk_begin(nuklearContext, nuklearWindow->GetWindowTitle(), nk_rect(0, 0, *windowWidth_, *windowHeight_), NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR))
			{
				if (!RenderWindowBody(nuklearContext, nullptr, nuklearWindow)) return false;
			}//End nk_begin
			nk_end(nuklearContext);
		}//End else if

		return true;
	}//End RenderWindow

public:
	NuklearWindowManager(shared_ptr<DataManager> dataManager, const nk_context* nuklearContext) : dataManager_(std::move(dataManager))
	{
		//Create the custom button styles to be used throughout the program
		CreateStyles(nuklearContext);

		//Create landing window
		CreateNewWindow("Data Authoring Tool", WINDOW_TYPE::LANDING);
	}//End constructor

	bool RenderAllActiveWindows(nk_context* nuklearContext)
	{
		const int startCount = activeNuklearWindows_.size();
		const NuklearWindow* frontWindow = activeNuklearWindows_.back();
		for (NuklearWindow* window : activeNuklearWindows_)
		{
			if (const bool success = RenderWindow(nuklearContext, window); !success) return false;
			if (activeNuklearWindows_.size() != startCount || frontWindow != activeNuklearWindows_.back()) return true;
		}//End for
		return true;
	}//End RenderAllActiveWindows

	//Creates a new empty window of given type
	bool CreateNewWindow(const char* windowTitle, const WINDOW_TYPE windowType)
	{
		bool memberWindowNoID = false;
		bool groupWindowNoID = false;
		bool templateWindowNoID = false;

		//Check active windows to see if an ID hasn't been assigned to the data being worked on
		for (const auto& activeNuklearWindow : activeNuklearWindows_)
		{
			switch (activeNuklearWindow->GetWindowType())
			{
			case WINDOW_TYPE::MEMBER_WINDOW:
				if (strcmp(activeNuklearWindow->GetWindowData()->GetIDBuffer(), "") == 0)
				{
					memberWindowNoID = true;
				}//End if
				break;
			case WINDOW_TYPE::GROUP_WINDOW:
				if (strcmp(activeNuklearWindow->GetWindowData()->GetIDBuffer(), "") == 0)
				{
					groupWindowNoID = true;
				}//End if
				break;
			case WINDOW_TYPE::TEMPLATE_WINDOW:
				if (strcmp(activeNuklearWindow->GetWindowData()->GetIDBuffer(), "") == 0)
				{
					templateWindowNoID = true;
				}//End if
				break;

			case WINDOW_TYPE::NONE: case WINDOW_TYPE::LANDING: default: continue;
			}//End switch
		}//End for

		switch (windowType)
		{
		case WINDOW_TYPE::MEMBER_WINDOW:
			if (!memberWindowNoID)
			{
				activeNuklearWindows_.push_back(new NuklearWindow(windowType, dataManager_, make_shared<Member>(Member(DataManager::GetInternalID(true))), windowTitle));
			}//End if
			break;
		case WINDOW_TYPE::GROUP_WINDOW:
			if (!groupWindowNoID)
			{
				activeNuklearWindows_.push_back(new NuklearWindow(windowType, dataManager_, make_shared<Group>(Group(DataManager::GetInternalID(true))), windowTitle));
			}//End if
			break;
		case WINDOW_TYPE::TEMPLATE_WINDOW:
			if (!templateWindowNoID)
			{
				activeNuklearWindows_.push_back(new NuklearWindow(windowType, dataManager_, make_shared<Template>(Template(DataManager::GetInternalID(true))), windowTitle));
			}//End if
			break;
		case WINDOW_TYPE::LANDING:
			activeNuklearWindows_.push_back(new NuklearWindow(windowType, nullptr));
			break;
		case WINDOW_TYPE::NONE: return false;
		}//End switch

		return true;
	}//End CreateNewWindow

	//Creates a window with existing data of given type
	bool CreateNewWindow(const char* windowTitle, const WINDOW_TYPE windowType, const shared_ptr<PrimaryData>& windowData, nk_context* nuklearContext)
	{
		if(windowData->GetInternalID() == -1) return false;

		for (auto it = activeNuklearWindows_.begin(); it != activeNuklearWindows_.end(); ++it)
		{
			if((*it)->GetWindowType() != WINDOW_TYPE::LANDING && (*it)->GetWindowTitle() == windowTitle && (*it)->GetWindowData() == windowData)
			{
				//Move the window to render on top
				string id;
				id.append((*it)->GetWindowTitle());
				const auto internalID = std::to_string((*it)->GetWindowData()->GetInternalID());
				id.append(internalID);
				nk_window_set_focus(nuklearContext, id.c_str());
				return true;
			}//End if
		}//End for

		activeNuklearWindows_.push_back(new NuklearWindow(windowType, dataManager_, windowData, windowTitle));
		return true;
	}//End CreateNewWindow

	//Used to keep the landing subwindow the same size as the program window
	void UpdateWindowSize(const long windowWidth, const long windowHeight) const
	{
		*windowWidth_ = windowWidth;
		*windowHeight_ = windowHeight;
	}//End UpdateWindowSize
};
#endif