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
	MemberField(Member* parentMember, DataType::DATA_TYPE dataType, int fieldIndex);
	int Validate() override;
	int Delete() override;

	[[nodiscard]] const char* GetNameAndTypeLabel() const;
	[[nodiscard]] const char* GetName() const;
	[[nodiscard]] const char* GetTypeLabel() const;
	DataType::DATA_TYPE GetDataType() const { return dataType_; }

	[[nodiscard]] char* GetDataBuffer() const { return field_->dataBuffer_; }
	[[nodiscard]] int* GetDataBufferCurrentSize() const { return &field_->dataBufferCurrentSize_; }
	[[nodiscard]] const int* GetDataBufferMaxSize() const { return &field_->maxDataBufferSize_; }
	[[nodiscard]] bool* GetBooleanData() const { return &field_->booleanData; }

private:
	shared_ptr<Field> field_ = nullptr;
	string fieldName_;
	int fieldIndex_;
	DataType::DATA_TYPE dataType_;
	Member* parentMember_ = nullptr;
	string parentMemberID_;
};
#endif