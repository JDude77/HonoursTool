#include "MemberField.h"
#include "Member.h"

MemberField::MemberField(Member* parentMember, DataType::DATA_TYPE dataType) : dataType_(dataType), parentMember_(parentMember)
{
	string parentID;
	parentID.append(parentMember->GetIDBuffer());
	parentMemberID_ = parentID;
}//End Constructor

int MemberField::Validate()
{
	//TODO Validate functionality for Member Field
	return 1;
}//End Validate

int MemberField::Delete()
{
	//Clear member field buffer
	memset(field_->dataBuffer_, 0, sizeof field_->dataBuffer_);
	return 1;
}//End Delete

const char* MemberField::GetNameAndTypeLabel()
{
	string nameAndType;
	nameAndType.append(parentMember_->GetNameBuffer());
	nameAndType.append(" - ");
	nameAndType.append(DataType::typeLabels_[dataType_]);
	const auto size = nameAndType.length();
	auto nameAndTypeArray = new char[size + 1];
	strcpy(nameAndTypeArray, nameAndType.c_str());

	return nameAndTypeArray;
}//End GetNameAndTypeLabel

const char* MemberField::GetName()
{
	string name;
	name.append(parentMember_->GetNameBuffer());
	const auto size = name.length();
	auto nameArray = new char[size + 1];
	strcpy(nameArray, name.c_str());

	return nameArray;
}//End GetName

const char* MemberField::GetTypeLabel()
{
	string type;
	type.append(DataType::typeLabels_[dataType_]);
	const auto size = type.length();
	auto typeArray = new char[size + 1];
	strcpy(typeArray, type.c_str());

	return typeArray;
}//End GetTypeLabel