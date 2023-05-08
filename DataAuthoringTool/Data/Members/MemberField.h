#pragma once
#ifndef MEMBER_FIELD
#define MEMBER_FIELD

#include "../Shared/DataInterfaces.h"
#include "../Shared/DataTypes.h"
#include "../Shared/Field.h"
#include <memory>

using std::shared_ptr;

//Forward Declarations
class Member;

class MemberField : public IDelete, public IValidate
{
public:
	MemberField(Member* parentMember, DataType::DATA_TYPE dataType, int fieldIndex);
	int Validate(std::queue<std::string>* outputText) override;
	int Delete() override;

	[[nodiscard]] const char* GetNameAndTypeLabel() const;
	[[nodiscard]] const char* GetName() const;
	[[nodiscard]] const char* GetTypeLabel() const;
	[[nodiscard]] DataType::DATA_TYPE GetDataType() const { return dataType_; }

	[[nodiscard]] char* GetDataBuffer() const { return field_->dataBuffer_; }
	[[nodiscard]] int* GetDataBufferCurrentSize() const { return &field_->dataBufferCurrentSize_; }
	[[nodiscard]] const int* GetDataBufferMaxSize() const { return &field_->maxDataBufferSize_; }
	[[nodiscard]] bool* GetBooleanData() const { return &field_->booleanData; }

	void SetDataBufferCurrentSize(const int size) const { field_->dataBufferCurrentSize_ = size; }

	void RefreshName();
	void RefreshType();

private:
	shared_ptr<Field> field_ = nullptr;
	string fieldName_;
	int fieldIndex_;
	DataType::DATA_TYPE dataType_;
	Member* parentMember_ = nullptr;
	string parentMemberID_;
};
#endif