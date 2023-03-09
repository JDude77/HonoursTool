#pragma once
#ifndef MEMBER_FIELD
#define MEMBER_FIELD

#include "../Shared/DataInterfaces.h"
#include "../Shared/DataTypes.h"
#include "../Shared/Field.h"

#include <memory>

class Member;
using std::shared_ptr;

class MemberField : public IDelete, public IValidate
{
public:
	MemberField(Member* parentMember, DataType::DATA_TYPE dataType);
	int Validate() override;
	int Delete() override;

	const char* GetNameAndTypeLabel();
	const char* GetName();
	const char* GetTypeLabel();
	DataType::DATA_TYPE GetDataType() { return dataType_; }

	char* GetDataBuffer(){ return field_->dataBuffer_; }
	int* GetDataBufferCurrentSize(){ return &field_->dataBufferCurrentSize_; }
	const int* GetDataBufferMaxSize(){ return &field_->maxDataBufferSize_; }
	bool* GetBooleanData(){ return &field_->booleanData; }

private:
	DataType::DATA_TYPE dataType_;
	shared_ptr<Field> field_ = nullptr;
	Member* parentMember_ = nullptr;
	string parentMemberID_;
};
#endif