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
	inline static shared_ptr<vector<Template>> cachedTemplates_;

	shared_ptr<DataManager> dataManager_;
	vector<NuklearWindow*> activeNuklearWindows_;

	void RenderTemplateHeaderDropdown(nk_context* nuklearContext, const shared_ptr<Member>& memberData)
	{
		//Update cached templates
		//TODO: Refactor this to not happen every frame of rendering a member window - only on templates updating
		cachedTemplates_ = make_shared<vector<Template>>(dataManager_->GetAllTemplates());

		int cachedTemplateIndex = memberData->GetTemplateIndex();

		vector<const char*> templateNames;
		for (auto& temp : *cachedTemplates_)
		{
			templateNames.push_back(temp.GetNameBuffer());
		}//End for
		
		//Render templates dropdown
		memberData->SetTemplateIndex(nk_combo(nuklearContext, templateNames.data(), templateNames.size(), memberData->GetTemplateIndex(), 24, nk_vec2(300, 300)));

		//Set template depending on index
		if(cachedTemplateIndex != memberData->GetTemplateIndex()) memberData->SetType(make_shared<Template>(cachedTemplates_->at(memberData->GetTemplateIndex())));
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
			if(nk_button_label(nuklearContext, "EXPORT")) windowData->Export(windowData.get());
			if(nk_button_label(nuklearContext, "DELETE")) windowData->Delete();

		return true;
	}//End RenderWindowFooter

	void PreventNegativeAndNonNegativeRule(TemplateField* field, const int j)
	{
		if(field->GetValidationRules()->at(j).first == NUMBER_IS_NEGATIVE)
		{
			if(field->GetValidationRules()->at(j-1).second == 1 && field->GetValidationRules()->at(j).second == 1)
			{
				field->GetValidationRules()->at(j-1).second = 0;
			}//End if
		}//End if
		if(field->GetValidationRules()->at(j).first == NUMBER_IS_NOT_NEGATIVE)
		{
			if(field->GetValidationRules()->at(j+1).second == 1 && field->GetValidationRules()->at(j).second == 1)
			{
				field->GetValidationRules()->at(j+1).second = 0;
			}//End if
		}//End if
	}//End PreventNegativeAndNonNegativeRule

	bool RenderWindowBody(nk_context* nuklearContext, const shared_ptr<PrimaryData>& windowData)
	{
		#pragma region Landing
		//LANDING
		if(windowData == nullptr)
		{
			//Template Button

			//Member Button

			//Group Button
		}//End if
		#pragma endregion

		#pragma region Member
		//MEMBER
		else if(const auto memberData = dynamic_pointer_cast<Member>(windowData))
		{
			for (int i = 0; i < memberData->GetNumberOfFields(); i++)
			{
				MemberField* field = memberData->GetFieldAtIndex(i);

				nk_layout_row_dynamic(nuklearContext, 24, 1);
					//Name of field label followed by type
					nk_label(nuklearContext, field->GetNameAndTypeLabel(), NK_LEFT);

				nk_layout_row_dynamic(nuklearContext, 24, 3);
					//Input field for data
					switch(field->GetDataType())
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
							nk_checkbox_label(nuklearContext, label, reinterpret_cast<nk_bool*>(field->GetBooleanData()));}
							break;

						case DataType::NONE:
							nk_label(nuklearContext, "ERROR: This field does not have a type! Please go to the template and choose a data type for this field.", NK_LEFT);
					}//End switch

					if(field->GetDataType() != DataType::BOOLEAN)
					{
						//Delete/Clear button for field
						if(nk_button_label(nuklearContext, "CLEAR FIELD")) field->Delete();

						//Validate button for field
						if(nk_button_label(nuklearContext, "VALIDATE")) field->Validate();
					}//End if
					else
					{
						//Two blank labels to fill out columns
						nk_label(nuklearContext, "", NK_LEFT);
						nk_label(nuklearContext, "", NK_LEFT);
					}//End else
			}//End for
		}//End else if
		#pragma endregion

		#pragma region Group
		//GROUP
		else if(const auto groupData = dynamic_pointer_cast<Group>(windowData))
		{
			//Future me problem
			if(nk_button_label(nuklearContext, "Debug: Update Group In Data Manager")) dataManager_->UpdateInstance(groupData.get());
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
					//Field Type
					field->SetDataType(nk_combo(nuklearContext, DataType::typeLabels_, std::size(DataType::typeLabels_), *field->GetDataTypeAsInt(), 24, nk_vec2(300, 300)));
					//Validation rules multi-select combo box
					//Boolean type and none type have no validation rules
					if(*field->GetDataType() != DataType::NONE && *field->GetDataType() != DataType::BOOLEAN)
					{
						if(nk_combo_begin_label(nuklearContext, "Click To Choose Validation Rules", nk_vec2(500, 300)))
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

					//Validation rule fill-out
					for(int j = 0; j < field->GetValidationRules()->size(); j++)
					{
						if(const auto [rule, active] = field->GetValidationRules()->at(j); active != 0)
						{
							switch(rule)
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
												field->GetValidationRuleParameters()->GetBuffer(rule),
												field->GetValidationRuleParameters()->GetBufferCurrentLength(rule),
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
												field->GetValidationRuleParameters()->GetBuffer(rule),
												field->GetValidationRuleParameters()->GetBufferCurrentLength(rule),
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
												field->GetValidationRuleParameters()->GetBuffer(rule),
												field->GetValidationRuleParameters()->GetBufferCurrentLength(rule),
												ValidationRuleParameter::stringStartsWithSubstringBufferSize_,
												nk_filter_decimal);
									}//End string starts with substring
									break;
								case STRING_ENDS_WITH_SUBSTRING:
									{
										nk_layout_row_dynamic(nuklearContext, 24, 2);
											nk_label(nuklearContext, "Text must end with the following: ", NK_TEXT_LEFT);
											nk_edit_string(nuklearContext,
												NK_EDIT_SIMPLE,
												field->GetValidationRuleParameters()->GetBuffer(rule),
												field->GetValidationRuleParameters()->GetBufferCurrentLength(rule),
												ValidationRuleParameter::stringEndsWithSubstringBufferSize_,
												nk_filter_decimal);
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
												field->GetValidationRuleParameters()->GetBuffer(rule),
												field->GetValidationRuleParameters()->GetBufferCurrentLength(rule),
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
												field->GetValidationRuleParameters()->GetBuffer(rule),
												field->GetValidationRuleParameters()->GetBufferCurrentLength(rule),
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
												field->GetValidationRuleParameters()->GetBuffer(rule),
												field->GetValidationRuleParameters()->GetBufferCurrentLength(rule),
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
												field->GetValidationRuleParameters()->GetBuffer(rule),
												field->GetValidationRuleParameters()->GetBufferCurrentLength(rule),
												ValidationRuleParameter::charIsOneOfCharacterSetBufferSize_,
												nk_filter_default);
									}//End char is one of character set
									break;
								case NA: default: ;
							}//End switch
						}//End if
					}//End for

					//Delete Field Button
					nk_layout_row_dynamic(nuklearContext, 24, 1);
						if(nk_button_label(nuklearContext, "DELETE FIELD")) field->Delete();
			}//End for

			//Add Field Button
			nk_layout_row_dynamic(nuklearContext, 24, 1);
				if(nk_button_label(nuklearContext, "ADD FIELD")) templateData->AddNewField();
				
			dataManager_->UpdateInstance(templateData.get());
		}//End else if
		#pragma endregion

		return true;
	}//End RenderWindowBody

	bool RenderWindow(nk_context* nuklearContext, NuklearWindow* nuklearWindow)
	{
		shared_ptr<PrimaryData> windowData = nuklearWindow->GetWindowData();
		if(windowData != nullptr)
		{
			if(nk_begin(nuklearContext, nuklearWindow->GetWindowTitle(), nk_rect(25, 50, 400, 400),
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
			}//End nk_begin
			nk_end(nuklearContext);
		}//End if

		//Landing window has no data, as it is essentially just buttons to open subwindows
		else if(nuklearWindow->GetWindowType() == LANDING)
		{
			if(nk_begin(nuklearContext, nuklearWindow->GetWindowTitle(), nk_rect(0, 0, 500, 500),
				NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
				NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
			{
				if(!RenderWindowBody(nuklearContext, nullptr)) return false;
			}//End nk_begin
			nk_end(nuklearContext);
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
		CreateNewWindow("Data Authoring Tool", LANDING);

		//Create template window
		CreateNewWindow("Test Template Window", TEMPLATE_WINDOW);

		//Create member window
		CreateNewWindow("Test Member Window", MEMBER_WINDOW);
	}//End constructor

	bool RenderAllActiveWindows(nk_context* nuklearContext)
	{
		for (NuklearWindow* window : activeNuklearWindows_)
		{
			if(const bool success = RenderWindow(nuklearContext, window); !success) return false;
		}//End for
		return true;
	}//End RenderAllActiveWindows
	
	bool CreateNewWindow(const char* windowTitle, const WINDOW_TYPE windowType)
	{
		switch(windowType)
		{
		case MEMBER_WINDOW:
				activeNuklearWindows_.push_back(new NuklearWindow(windowType, dataManager_, make_shared<Member>(Member(DataManager::GetInternalID(true)))));
				break;
			case GROUP_WINDOW:
				activeNuklearWindows_.push_back(new NuklearWindow(windowType, dataManager_, make_shared<Group>(Group(DataManager::GetInternalID(true)))));
				break;
			case TEMPLATE_WINDOW:
				activeNuklearWindows_.push_back(new NuklearWindow(windowType, dataManager_, make_shared<Template>(Template(DataManager::GetInternalID(true)))));
				break;
			case LANDING: 
				activeNuklearWindows_.push_back(new NuklearWindow(windowType, nullptr));
				break;
			case NONE: return false;
		}//End switch
		
		return true;
	}//End CreateNewWindow
};
#endif