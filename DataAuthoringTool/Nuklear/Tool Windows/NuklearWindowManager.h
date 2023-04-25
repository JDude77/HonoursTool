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

class NuklearWindowManager
{
private:
	inline static shared_ptr<vector<shared_ptr<Template>>> cachedTemplates_;

	shared_ptr<DataManager> dataManager_;
	vector<NuklearWindow*> activeNuklearWindows_;

	shared_ptr<float> windowWidth_ = make_shared<float>(0.0f);
	shared_ptr<float> windowHeight_ = make_shared<float>(0.0f);

	void RenderTemplateHeaderDropdown(nk_context* nuklearContext, const shared_ptr<Member>& memberData)
	{
		//Update cached templates
		//TODO: Refactor this to not happen every frame of rendering a member window - only on templates updating
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

	bool RenderWindowHeader(nk_context* nuklearContext, const shared_ptr<PrimaryData>& windowData)
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
			if (nk_button_label(nuklearContext, "VALIDATE ALL")) memberData->Validate();
		}//End if

		//If the data we're handling is a group
		else if (const auto groupData = dynamic_pointer_cast<Group>(windowData))
		{
			//Validate All Members button
			nk_layout_row_dynamic(nuklearContext, 24, 1);
			if (nk_button_label(nuklearContext, "VALIDATE")) groupData->Validate();
		}//End else if

		return true;
	}//End RenderWindowHeader

	bool RenderWindowFooter(nk_context* nuklearContext, const shared_ptr<PrimaryData>& windowData)
	{
		nk_layout_row_dynamic(nuklearContext, 24, 1);
		if (nk_button_label(nuklearContext, "SAVE")) windowData->Save();
		if (nk_button_label(nuklearContext, "LOAD")) windowData->Load();
		if (nk_button_label(nuklearContext, "EXPORT")) windowData->Export(windowData.get());
		if (nk_button_label(nuklearContext, "DELETE")) windowData->Delete();

		return true;
	}//End RenderWindowFooter

	static void PreventNegativeAndNonNegativeRule(TemplateField* field, const int j)
	{
		//Check to see if the number is negative rule has just been activated
		if (field->GetValidationRules()->at(j).first == NUMBER_IS_NEGATIVE)
		{
			//If the number is not negative rule is ALSO active
			if (field->GetValidationRules()->at(j - 1).second == 1 && field->GetValidationRules()->at(j).second == 1)
			{
				//Deactivate the number is not negative rule
				field->GetValidationRules()->at(j - 1).second = 0;
			}//End if
		}//End if

		//Check to see if the number is not negative rule has just been activated
		if (field->GetValidationRules()->at(j).first == NUMBER_IS_NOT_NEGATIVE)
		{
			//If the number is negative rule is ALSO active
			if (field->GetValidationRules()->at(j + 1).second == 1 && field->GetValidationRules()->at(j).second == 1)
			{
				//Deactivate the number is negative rule
				field->GetValidationRules()->at(j + 1).second = 0;
			}//End if
		}//End if
	}//End PreventNegativeAndNonNegativeRule

	static void ClearDeletedCharacters(char* idBuffer, const int idBufferCurrentLength)
	{
		//Check to see if the current buffer has more characters in it than there should be
		if (strlen(idBuffer) > idBufferCurrentLength)
		{
			//Loop through every value after the end of the expected length of the buffer
			for (int j = idBufferCurrentLength; j < TemplateField::GetBufferMax(); j++)
			{
				//Replace with a null character if it isn't already null - if it is, stop
				if (idBuffer[j] == '\0') break;
				idBuffer[j] = '\0';
			}//End for
		}//End if
	}//End ClearDeletedCharacters

	bool RenderGroupTemplateTabs(nk_context* nuklearContext, const shared_ptr<Group>& groupData, int& activeTab, bool& value)
	{
		cachedTemplates_ = make_shared<vector<shared_ptr<Template>>>(dataManager_->GetAllTemplates());

		//Get the number of tabs that should be active
		const int numberOfTabs = groupData->GetNumberOfTemplates();

		//Get list of names for templates
		vector<const char*> templateNames;
		for (const auto& temp : *cachedTemplates_)
		{
			if (temp->GetInternalID() == -1 || temp->IsEmpty())
				continue;

			auto cache = groupData->GetTemplates();
			if (std::ranges::find(cache, temp) != cache.end())
				continue;

			templateNames.push_back(temp->GetIDBuffer());
		}//End for

		//Only show the ability to add templates if there are templates to add
		//Set up the tab styling
		nk_style_push_vec2(nuklearContext, &nuklearContext->style.window.spacing, nk_vec2(0, 0));
		nk_style_push_float(nuklearContext, &nuklearContext->style.button.rounding, 0);
		nk_layout_row_dynamic(nuklearContext, 24, numberOfTabs + 1);

		//Tab with template listing added members
		for (int i = 0; i < groupData->GetNumberOfTemplates(); i++)
		{
			if (const auto templateLabel = groupData->GetTemplates().at(i)->GetIDBuffer(); nk_selectable_tab(nuklearContext, templateLabel, activeTab == i)) { activeTab = i; }
		}//End for

		if (!templateNames.empty())
		{
			//Dropdown/button combo which selects and adds template at the same time
			if (nk_combo_begin_label(nuklearContext, "Add Template To Group", nk_vec2(300, 300)))
			{
				vector<const char*> groupTemplateNames;

				for (const auto& temp : groupData->GetTemplates())
				{
					groupTemplateNames.push_back(temp->GetIDBuffer());
				}//End for

				nk_layout_row_dynamic(nuklearContext, 24, 1);

				//Render every button of templates to add
				for (int i = 0; i < templateNames.size(); i++)
				{
					//If the template is not already within the group
					if (auto& name = templateNames.at(i); std::ranges::find(groupTemplateNames, name) == groupTemplateNames.end())
					{
						if (nk_button_label(nuklearContext, templateNames.at(i))) { groupData->SetActiveTemplateTab(i); break; }
					}//End if
				}//End for

				if (static int* activeTemplateTab = groupData->GetActiveTemplateTab(); *activeTemplateTab != -1)
				{
					groupData->AddTemplateToGroup(dataManager_->GetAllTemplates().at(*activeTemplateTab + 1));
					groupData->SetActiveTemplateTab(-1);
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
		nk_style_pop_float(nuklearContext);
		nk_style_pop_vec2(nuklearContext);

		return false;
	}//End RenderGroupTemplateTabs

	bool RenderWindowBody(nk_context* nuklearContext, const shared_ptr<PrimaryData>& windowData)
	{
#pragma region Landing
		//LANDING
		if (windowData == nullptr)
		{
			//Template Button
			nk_layout_space_begin(nuklearContext, NK_DYNAMIC, *windowHeight_ / 6.0f, 3);
				nk_layout_space_push(nuklearContext, nk_rect(0.01f, 0.1f, 0.45f, 0.45f));
					if (nk_button_label(nuklearContext, "Open New Template Window"))
						CreateNewWindow("Template Window", TEMPLATE_WINDOW);
				nk_layout_space_push(nuklearContext, nk_rect(0.475f, 0.05f, 0.55f, 0.25f));
					nk_label(nuklearContext, "Templates", NK_TEXT_LEFT);
				nk_layout_space_push(nuklearContext, nk_rect(0.475f, 0.275f, 0.55f, 0.45f));
					nk_label_wrap(nuklearContext, "The blueprints you can use to define Member types, as well as set up validation rules.");
			nk_layout_space_end(nuklearContext);

			//Member Button
			nk_layout_space_begin(nuklearContext, NK_DYNAMIC, *windowHeight_ / 6.0f, 3);
				nk_layout_space_push(nuklearContext, nk_rect(0.01f, 0.1f, 0.45f, 0.45f));
					if (nk_button_label(nuklearContext, "Open New Member Window"))
						CreateNewWindow("Member Window", MEMBER_WINDOW);
				nk_layout_space_push(nuklearContext, nk_rect(0.475f, 0.05f, 0.55f, 0.25f));
					nk_label(nuklearContext, "Members", NK_TEXT_LEFT);
				nk_layout_space_push(nuklearContext, nk_rect(0.475f, 0.275f, 0.55f, 0.45f));
					nk_label_wrap(nuklearContext, "The objects you can create, using a Template as the guide for how everything is laid out and validated.");
			nk_layout_space_end(nuklearContext);

			//Template Button
			nk_layout_space_begin(nuklearContext, NK_DYNAMIC, *windowHeight_ / 6.0f, 3);
				nk_layout_space_push(nuklearContext, nk_rect(0.01f, 0.1f, 0.45f, 0.45f));
					if (nk_button_label(nuklearContext, "Open New Group Window"))
						CreateNewWindow("Group Window", GROUP_WINDOW);
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
								CreateNewWindow("Template Window", TEMPLATE_WINDOW, templateData, nuklearContext);
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
								CreateNewWindow("Member Window", MEMBER_WINDOW, memberData, nuklearContext);
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
								CreateNewWindow("Group Window", GROUP_WINDOW, groupData, nuklearContext);
						}//End for
						nk_group_end(nuklearContext);
					}//End if

			nk_layout_space_end(nuklearContext);
		}//End if
#pragma endregion

#pragma region Member
		//MEMBER
		else if (const auto memberData = dynamic_pointer_cast<Member>(windowData))
		{
			for (int i = 0; i < memberData->GetNumberOfFields(); i++)
			{
				MemberField* field = memberData->GetFieldAtIndex(i);

				nk_layout_row_dynamic(nuklearContext, 24, 1);
				//Name of field label followed by type
				nk_label(nuklearContext, field->GetNameAndTypeLabel(), NK_LEFT);

				nk_layout_row_dynamic(nuklearContext, 24, 3);
				//Input field for data
				switch (field->GetDataType())
				{
				case DataType::STRING:
					//TODO: Add functionality for applying length limit validation rule to max length
					nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field->GetDataBuffer(), field->GetDataBufferCurrentSize(), *field->GetDataBufferMaxSize(), nk_filter_default);
					break;

				case DataType::INTEGER:
					nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field->GetDataBuffer(), field->GetDataBufferCurrentSize(), *field->GetDataBufferMaxSize(), nk_filter_decimal);
					break;

				case DataType::FLOAT:
					nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field->GetDataBuffer(), field->GetDataBufferCurrentSize(), *field->GetDataBufferMaxSize(), nk_filter_float);
					break;

				case DataType::CHAR:
					nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field->GetDataBuffer(), field->GetDataBufferCurrentSize(), 2, nk_filter_default);
					break;

				case DataType::BOOLEAN:
				{const char* label = *field->GetBooleanData() ? "TRUE" : "FALSE";
				nk_checkbox_label(nuklearContext, label, reinterpret_cast<nk_bool*>(field->GetBooleanData())); }
				break;

				case DataType::NONE:
					nk_label(nuklearContext, "ERROR: This field does not have a type! Please go to the template and choose a data type for this field.", NK_LEFT);
				}//End switch

				if (field->GetDataType() != DataType::BOOLEAN)
				{
					//Delete/Clear button for field
					if (nk_button_label(nuklearContext, "CLEAR FIELD")) field->Delete();

					//Validate button for field
					if (nk_button_label(nuklearContext, "VALIDATE")) field->Validate();
				}//End if
				else
				{
					//Two blank labels to fill out columns
					nk_label(nuklearContext, "", NK_LEFT);
					nk_label(nuklearContext, "", NK_LEFT);
				}//End else
			}//End for
			dataManager_->UpdateInstance(memberData);
		}//End else if
#pragma endregion

#pragma region Group
		//GROUP
		else if (const auto groupData = dynamic_pointer_cast<Group>(windowData))
		{
			static int activeTab = 0;

			//Render the tabs to let you select which template is currently active
			//Needs a formatting update but good enough for now
			if (bool tabsUpdated; RenderGroupTemplateTabs(nuklearContext, groupData, activeTab, tabsUpdated)) return tabsUpdated;
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

				if (nk_button_label(nuklearContext, "VALIDATE"))
				{
					member->Validate();
				}//End if
				if (nk_button_label(nuklearContext, "REMOVE"))
				{
					groupData->RemoveMemberFromGroup(member);
				}//End if
			}//End for

			//Add member button
			if (!groupData->GetTemplates().empty())
			{
				nk_layout_row_dynamic(nuklearContext, 24, 1);
				//Get only members which aren't considered empty
				static vector<shared_ptr<Member>> addableMembers;
				for (shared_ptr<Member>& member : dataManager_->GetAllMembers())
				{
					if (std::ranges::find(addableMembers, member) != addableMembers.end()) continue;
					addableMembers.push_back(member);
				}//End for

				//Cache all members from the data manager for sorting
				if (!addableMembers.empty())
				{
					//Similar button/dropdown system to the add template dropdown
					if (nk_combo_begin_label(nuklearContext, "ADD MEMBER", nk_vec2(300, 300)))
					{
						//Create the vector of sorted members
						vector<shared_ptr<Member>> allMembersOfTemplateNotAlreadyInGroup;
						//For every member
						for (auto& addableMember : addableMembers)
						{
							//If it doesn't have data, don't waste time on it
							if (addableMember->IsEmpty()) continue;

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
						for (auto& member : allMembersOfTemplateNotAlreadyInGroup)
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
								allMembersOfTemplateNotAlreadyInGroup.erase(allMembersOfTemplateNotAlreadyInGroup.begin(), allMembersOfTemplateNotAlreadyInGroup.end());
								addableMembers.erase(addableMembers.begin(), addableMembers.end());
								nk_combo_close(nuklearContext);
								return true;
							}//End if
						}//End for
						nk_combo_end(nuklearContext);
					}//End if
				}//End if
			}//End if

			dataManager_->UpdateInstance(groupData);
		}//End else if
#pragma endregion

#pragma region Template
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

				TemplateField* field = templateData->GetFieldAtIndex(i);
				//Field Name
				nk_edit_string(nuklearContext, NK_EDIT_SIMPLE, field->GetIDBuffer(), field->GetIDBufferCurrentLength(), TemplateField::GetBufferMax(), nk_filter_ascii);
				//Clear deleted characters
				ClearDeletedCharacters(field->GetIDBuffer(), *field->GetIDBufferCurrentLength());

				//Field Type
				field->SetDataType(nk_combo(nuklearContext, DataType::typeLabels_, std::size(DataType::typeLabels_), *field->GetDataTypeAsInt(), 24, nk_vec2(300, 300)));
				//Validation rules multi-select combo box
				//Boolean type and none type have no validation rules
				if (*field->GetDataType() != DataType::NONE && *field->GetDataType() != DataType::BOOLEAN)
				{
					if (nk_combo_begin_label(nuklearContext, "Click To Choose Validation Rules", nk_vec2(500, 300)))
					{
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
				if (nk_button_label(nuklearContext, "DELETE FIELD")) field->Delete();

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
							if (validationRuleParameters.GetBuffer(rule) != nullptr) ClearDeletedCharacters(validationRuleParameters.GetBuffer(rule), *validationRuleParameters.GetBufferCurrentLength(rule));
						}//End if
					}//End for
					nk_tree_pop(nuklearContext);
				}//End if
			}//End for

			//Add Field Button
			nk_layout_row_dynamic(nuklearContext, 24, 1);
			if (nk_button_label(nuklearContext, "ADD FIELD")) templateData->AddNewField();

			dataManager_->UpdateInstance(templateData);
		}//End else if
#pragma endregion

		return true;
	}//End RenderWindowBody

	bool RenderWindow(nk_context* nuklearContext, NuklearWindow* nuklearWindow)
	{
		shared_ptr<PrimaryData> windowData = nuklearWindow->GetWindowData();

		if (windowData != nullptr)
		{
			string id;
			id.append(nuklearWindow->GetWindowTitle());
			id.append(std::to_string(nuklearWindow->GetWindowData()->GetInternalID()));
			if (nk_begin_titled(nuklearContext, id.c_str(), nuklearWindow->GetWindowTitle(), nk_rect(25, 50, 400, 400),
				NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
				NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_WINDOW_CLOSABLE))
			{

				//Render Header
				if (nuklearWindow->GetHasHeader())
				{
					if (!RenderWindowHeader(nuklearContext, windowData)) return false;
				}//End if

				//Render Body
				if (!RenderWindowBody(nuklearContext, windowData)) return false;

				//Render Footer
				if (nuklearWindow->GetHasFooter())
				{
					if (!RenderWindowFooter(nuklearContext, windowData)) return false;
				}//End if
			}//End nk_begin

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
					return false;
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
					return false;
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
					return false;
				}//End if
			}//End if

			nk_end(nuklearContext);
		}//End if

		//Landing window has no data, as it is essentially just buttons to open subwindows
		else if (nuklearWindow->GetWindowType() == LANDING)
		{
			if (nk_begin(nuklearContext, nuklearWindow->GetWindowTitle(), nk_rect(0, 0, *windowWidth_, *windowHeight_), NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR))
			{
				if (!RenderWindowBody(nuklearContext, nullptr)) return false;
			}//End nk_begin
			nk_end(nuklearContext);
		}//End else if

		return true;
	}//End RenderWindow

public:
	NuklearWindowManager(shared_ptr<DataManager> dataManager) : dataManager_(std::move(dataManager))
	{
		//Create landing window
		CreateNewWindow("Data Authoring Tool", LANDING);
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
			case MEMBER_WINDOW:
				if (strcmp(activeNuklearWindow->GetWindowData()->GetIDBuffer(), "") == 0)
				{
					memberWindowNoID = true;
				}//End if
				break;
			case GROUP_WINDOW:
				if (strcmp(activeNuklearWindow->GetWindowData()->GetIDBuffer(), "") == 0)
				{
					groupWindowNoID = true;
				}//End if
				break;
			case TEMPLATE_WINDOW:
				if (strcmp(activeNuklearWindow->GetWindowData()->GetIDBuffer(), "") == 0)
				{
					templateWindowNoID = true;
				}//End if
				break;

			case NONE: case LANDING: default: continue;
			}//End switch
		}//End for

		switch (windowType)
		{
		case MEMBER_WINDOW:
			if (!memberWindowNoID)
			{
				activeNuklearWindows_.push_back(new NuklearWindow(windowType, dataManager_, make_shared<Member>(Member(DataManager::GetInternalID(true))), windowTitle));
			}//End if
			break;
		case GROUP_WINDOW:
			if (!groupWindowNoID)
			{
				activeNuklearWindows_.push_back(new NuklearWindow(windowType, dataManager_, make_shared<Group>(Group(DataManager::GetInternalID(true))), windowTitle));
			}//End if
			break;
		case TEMPLATE_WINDOW:
			if (!templateWindowNoID)
			{
				activeNuklearWindows_.push_back(new NuklearWindow(windowType, dataManager_, make_shared<Template>(Template(DataManager::GetInternalID(true))), windowTitle));
			}//End if
			break;
		case LANDING:
			activeNuklearWindows_.push_back(new NuklearWindow(windowType, nullptr));
			break;
		case NONE: return false;
		}//End switch

		return true;
	}//End CreateNewWindow

	bool CreateNewWindow(const char* windowTitle, const WINDOW_TYPE windowType, const shared_ptr<PrimaryData>& windowData, nk_context* nuklearContext)
	{
		if(windowData->GetInternalID() == -1) return false;

		for (auto it = activeNuklearWindows_.begin(); it != activeNuklearWindows_.end(); ++it)
		{
			if((*it)->GetWindowType() != LANDING && (*it)->GetWindowTitle() == windowTitle && (*it)->GetWindowData() == windowData)
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

	void UpdateWindowSize(const long windowWidth, const long windowHeight) const
	{
		*windowWidth_ = windowWidth;
		*windowHeight_ = windowHeight;
	}//End UpdateWindowSize
};
#endif