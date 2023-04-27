#include "MemberField.h"

#include <iostream>

#include "Member.h"
#include "../Templates/Template.h"

MemberField::MemberField(Member* parentMember, const DataType::DATA_TYPE dataType, const int fieldIndex) : fieldIndex_(fieldIndex), dataType_(dataType), parentMember_(parentMember)
{
	string parentID;
	parentID.append(parentMember->GetIDBuffer());
	parentMemberID_ = parentID;

	fieldName_.append(parentMember->GetType()->GetFieldAtIndex(fieldIndex_)->GetIDBuffer());
	field_ = std::make_shared<Field>();
}//End Constructor

int MemberField::Validate()
{
	const auto templateField = parentMember_->GetType()->GetFieldAtIndex(fieldIndex_);
	const auto validationRules = templateField->GetValidationRules();
	const auto validationParameters = templateField->GetValidationRuleParameters();

	for (auto& [rule, active] : *validationRules)
	{
		if(active != 0)
		{
			bool success;
			string text;
			switch (rule)
			{
				case ALL_PRESENCE:
					success = ValidationFunction::Presence(GetDataBuffer());
					text = "Presence Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success) text.append("successful! (Data Is Present)");
					else text.append("unsuccessful! (Data Is Missing)");
					break;

				case STRING_MAX_LENGTH:
					success = ValidationFunction::StringMaxLength(GetDataBuffer(), validationParameters->GetBuffer(rule));
					text = "String Max Length Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success) text.append("successful! (Text Is Shorter Than Max Length Of ");
					else text.append("unsuccessful! (Text Is Longer Than Max Length Of ");
					text.append(validationParameters->GetBuffer(rule));
					text.append(" Characters)");
					break;

				case STRING_MIN_LENGTH:
					success = ValidationFunction::StringMinLength(GetDataBuffer(), validationParameters->GetBuffer(rule));
					text = "String Min Length Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success) text.append("successful! (Text Is Longer Than Min Length Of ");
					else text.append("unsuccessful! (Text Is Shorter Than Min Length Of ");
					text.append(validationParameters->GetBuffer(rule));
					text.append(" Characters)");
					break;

				case STRING_STARTS_WITH_SUBSTRING:
					success = ValidationFunction::StringStartsWithSubstring(GetDataBuffer(), validationParameters->GetBuffer(rule));
					text = "String Starts With Substring Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success) text.append("successful! (Text Starts With Substring: \"");
					else text.append("unsuccessful! (Text Does Not Start With Substring: \"");
					text.append(validationParameters->GetBuffer(rule));
					text.append("\")");
					break;

				case STRING_ENDS_WITH_SUBSTRING:
					success = ValidationFunction::StringEndsWithSubstring(GetDataBuffer(), validationParameters->GetBuffer(rule));
					text = "String Ends With Substring Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success) text.append("successful! (Text Ends With Substring: \"");
					else text.append("unsuccessful! (Text Does Not End With Substring: \"");
					text.append(validationParameters->GetBuffer(rule));
					text.append("\")");
					break;

				case NUMBER_IS_NOT_NEGATIVE:
					success = ValidationFunction::NumberIsNotNegative(GetDataBuffer());
					text = "Number Is Not Negative Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success)
					{
						text.append("successful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Is Positive)");
					}//End if
					else 
					{
						text.append("unsuccessful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Is Negative)");
					}//End else
					break;

				case NUMBER_IS_NEGATIVE:
					success = ValidationFunction::NumberIsNegative(GetDataBuffer());
					text = "Number Is Negative Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success)
					{
						text.append("successful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Is Negative)");
					}//End if
					else 
					{
						text.append("unsuccessful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Is Positive)");
					}//End else
					break;

				case NUMBER_IS_NOT_ZERO:
					success = ValidationFunction::NumberIsNotZero(GetDataBuffer());
					text = "Number Is Not Zero Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success)
					{
						text.append("successful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Is Not Zero)");
					}//End if
					else 
					{
						text.append("unsuccessful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Is Zero)");
					}//End else
					break;

				case NUMBER_IS_LESS_THAN:
					success = ValidationFunction::NumberIsLessThan(GetDataBuffer(), validationParameters->GetBuffer(rule));
					text = "Number Is Less Than Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success)
					{
						text.append("successful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Is Less Than Validation Number ");
						text.append(validationParameters->GetBuffer(rule));
						text.append(")");
					}//End if
					else 
					{
						text.append("unsuccessful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Is Greater Than Or Equal To Validation Number ");
						text.append(validationParameters->GetBuffer(rule));
						text.append(")");
					}//End else
					break;

				case NUMBER_IS_GREATER_THAN:
					success = ValidationFunction::NumberIsGreaterThan(GetDataBuffer(), validationParameters->GetBuffer(rule));
					text = "Number Is Greater Than Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success)
					{
						text.append("successful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Is Greater Than Validation Number ");
						text.append(validationParameters->GetBuffer(rule));
						text.append(")");
					}//End if
					else 
					{
						text.append("unsuccessful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Is Less Than Or Equal To Validation Number ");
						text.append(validationParameters->GetBuffer(rule));
						text.append(")");
					}//End else
					break;

				case INTEGER_DIVISIBLE_BY_INTEGER:
					success = ValidationFunction::IntegerDivisibleByInteger(GetDataBuffer(), validationParameters->GetBuffer(rule));
					text = "Integer Divisible By Other Integer Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success)
					{
						text.append("successful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Divides Into ");
						text.append(validationParameters->GetBuffer(rule));
						text.append(" With No Remainder)");
					}//End if
					else 
					{
						text.append("unsuccessful! (Provided Number ");
						text.append(field_->dataBuffer_);
						text.append(" Does Not Divide Into ");
						text.append(validationParameters->GetBuffer(rule));
						text.append(" With No Remainder)");
					}//End else
					break;
				case CHAR_IS_ONE_OF_CHARACTER_SET:
					success = ValidationFunction::CharIsOneOfCharacterSet(GetDataBuffer(), validationParameters->GetBuffer(rule));
					text = "Character Is One Of Set Test on ";
					text.append(GetNameAndTypeLabel());
					text.append(" was ");
					if(success)
					{
						text.append("successful! (Provided Character ");
						text.append(field_->dataBuffer_);
						text.append(" Is One Of Accepted Values: ");
						text.append(validationParameters->GetBuffer(rule));
						text.append(")");
					}//End if
					else 
					{
						text.append("unsuccessful! (Provided Character ");
						text.append(field_->dataBuffer_);
						text.append(" Is Not One Of Accepted Values: ");
						text.append(validationParameters->GetBuffer(rule));
						text.append(")");
					}//End else
					break;
				case NA: default: success = true;
			}//End switch
			std::cout << text << std::endl;
		}//End if
	}//End for

	return 1;
}//End Validate

int MemberField::Delete()
{
	//Clear member field buffer
	memset(field_->dataBuffer_, '\0', sizeof field_->dataBuffer_);
	field_->dataBufferCurrentSize_ = 0;
	return 1;
}//End Delete

const char* MemberField::GetNameAndTypeLabel() const
{
	string nameAndType;
	nameAndType.append(fieldName_);
	nameAndType.append(" - ");
	nameAndType.append(DataType::typeLabels_[dataType_]);
	const auto size = nameAndType.length();
	const auto nameAndTypeArray = new char[size + 1];
	strcpy(nameAndTypeArray, nameAndType.c_str());

	return nameAndTypeArray;
}//End GetNameAndTypeLabel

const char* MemberField::GetName() const
{
	string name;
	name.append(fieldName_);
	const auto size = name.length();
	const auto nameArray = new char[size + 1];
	strcpy(nameArray, name.c_str());

	return nameArray;
}//End GetName

const char* MemberField::GetTypeLabel() const
{
	string type;
	type.append(DataType::typeLabels_[dataType_]);
	const auto size = type.length();
	const auto typeArray = new char[size + 1];
	strcpy(typeArray, type.c_str());

	return typeArray;
}//End GetTypeLabel

void MemberField::RefreshName()
{
	fieldName_.clear();
	fieldName_.append(parentMember_->GetType()->GetFieldAtIndex(fieldIndex_)->GetIDBuffer());
}//End RefreshName

void MemberField::RefreshType()
{
	const DataType::DATA_TYPE newType = *parentMember_->GetType()->GetFieldAtIndex(fieldIndex_)->GetDataType();

	//If there's no existing data in the buffer, safe to immediately switch
	if(strlen(field_->dataBuffer_) == 0)
	{
		dataType_ = newType;
		return;
	}//End if

	bool clearAll = true;

	//If there is data in the buffer, check for type compatibility
	switch(dataType_)
	{
		case DataType::STRING: 
			if(newType == DataType::CHAR)
			{
				clearAll = false;
				//Take the first character of the old data for the char switch
				const char first = field_->dataBuffer_[0];
				memset(field_->dataBuffer_, 0, sizeof(field_->dataBuffer_));
				field_->dataBuffer_[0] = first;
				field_->dataBufferCurrentSize_ = 2;
			}//End if
			break;
		case DataType::INTEGER:
			if(newType == DataType::FLOAT)
			{
				//Leave the data as is, it'll be fine
				clearAll = false;
			}//End if
			break;
		case DataType::FLOAT:
			if(newType == DataType::INTEGER)
			{
				//Stringify for easier function use
				string data = field_->dataBuffer_;
				//Take the data from before the decimal point
				const int pos = data.find('.');
				const string newData = data.substr(0, pos);
				if(!newData.empty())
				{
					clearAll = false;
					memset(field_->dataBuffer_, 0, sizeof(field_->dataBuffer_));
					strcpy(field_->dataBuffer_, newData.c_str());
					field_->dataBufferCurrentSize_ = newData.size();
				}//End if
			}//End if
			break;
		case DataType::CHAR:
			if(newType == DataType::STRING)
			{
				clearAll = false;
			}//End if
			break;

		case DataType::BOOLEAN: case DataType::NONE: default: ;
	}//End switch

	if(clearAll)
	{
		memset(field_->dataBuffer_, 0, sizeof field_->dataBuffer_);
		field_->dataBufferCurrentSize_ = 0;
	}//End if
	dataType_ = newType;
}//End RefreshType