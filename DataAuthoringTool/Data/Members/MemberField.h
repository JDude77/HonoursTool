#pragma once
#ifndef MEMBER_FIELD
#define MEMBER_FIELD

#include "../Shared/DataInterfaces.h"

#include <memory>

#include "../../Field.h"
using std::shared_ptr;

class MemberField : public IDelete, public IValidate
{
public:
	int Validate() override;
	int Delete() override;

	char* GetNameAndTypeLabel();
	char* GetName();
	char* GetTypeLabel();
	DataType::DATA_TYPE GetDataType();

	char* GetDataBuffer(){ return field_->dataBuffer_; }
	int* GetDataBufferCurrentSize(){ return &field_->dataBufferCurrentSize_; }
	const int* GetDataBufferMaxSize(){ return &field_->maxDataBufferSize_; }
	bool* GetBooleanData(){ return &field_->booleanData; }

private:
	shared_ptr<Field> field_ = nullptr;
};
#endif