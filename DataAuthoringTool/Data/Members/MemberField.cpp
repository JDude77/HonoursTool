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
			switch (rule)
			{
				case ALL_PRESENCE:
					success = ValidationFunction::Presence(GetDataBuffer());
					break;
				case STRING_MAX_LENGTH:
					success = ValidationFunction::StringMaxLength(GetDataBuffer(), validationParameters->GetBuffer(rule));
					break;
				case STRING_MIN_LENGTH:
					success = ValidationFunction::StringMinLength(GetDataBuffer(), validationParameters->GetBuffer(rule));
					break;
				case STRING_STARTS_WITH_SUBSTRING:
					success = ValidationFunction::StringStartsWithSubstring(GetDataBuffer(), validationParameters->GetBuffer(rule));
					break;
				case STRING_ENDS_WITH_SUBSTRING:
					success = ValidationFunction::StringEndsWithSubstring(GetDataBuffer(), validationParameters->GetBuffer(rule));
					break;
				case NUMBER_IS_NOT_NEGATIVE:
					success = ValidationFunction::NumberIsNotNegative(GetDataBuffer());
					break;
				case NUMBER_IS_NEGATIVE:
					success = ValidationFunction::NumberIsNegative(GetDataBuffer());
					break;
				case NUMBER_IS_NOT_ZERO:
					success = ValidationFunction::NumberIsNotZero(GetDataBuffer());
					break;
				case NUMBER_IS_LESS_THAN:
					success = ValidationFunction::NumberIsLessThan(GetDataBuffer(), validationParameters->GetBuffer(rule));
					break;
				case NUMBER_IS_GREATER_THAN:
					success = ValidationFunction::NumberIsGreaterThan(GetDataBuffer(), validationParameters->GetBuffer(rule));
					break;
			case INTEGER_DIVISIBLE_BY_INTEGER:
					success = ValidationFunction::IntegerDivisibleByInteger(GetDataBuffer(), validationParameters->GetBuffer(rule));
					break;
			case CHAR_IS_ONE_OF_CHARACTER_SET:
					success = ValidationFunction::CharIsOneOfCharacterSet(GetDataBuffer(), validationParameters->GetBuffer(rule));
					break;
				case NA: default: success = true;
			}
			std::cout << "Test " << toupper(rule) << " on " << fieldName_ << " had result " << success << std::endl;
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