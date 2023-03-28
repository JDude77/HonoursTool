#pragma once
#ifndef TEMPLATE_FIELD
#define TEMPLATE_FIELD

#include "../Shared/DataInterfaces.h"
#include "../Shared/DataTypes.h"
#include "../Validation/ValidationRule.h"
#include "../Validation/ValidationRuleParameter.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

//Forward Declarations
//class ValidationRule;

class TemplateField : public IDelete
{
public:
	int Delete() override;
	char* GetIDBuffer() { return idBuffer_; }
	int* GetIDBufferCurrentLength() { return &idBufferCurrentLength_; }
	void RefreshValidationRules(int type);
	void SetDataType(int type);
	void SetDataType(DataType::DATA_TYPE type);
	static int GetBufferMax() { return bufferMax_; }
	DataType::DATA_TYPE* GetDataType() { return &dataType_; }
	int* GetDataTypeAsInt() { return &typeInt_; }
	vector<std::pair<RULES, int>>* GetValidationRules(){ return &validationRules_; }

private:
	//Note: ID referred to in-program as "name", but called ID internally to indicate that each field in a template must have a unique name
	constexpr static int bufferMax_ = 64;

	char idBuffer_[bufferMax_] = {};
	int idBufferCurrentLength_ = 0;

	string id_;
	DataType::DATA_TYPE dataType_ = DataType::NONE;
	int typeInt_ = DataType::NONE;
	vector<std::pair<RULES, int>> validationRules_;
};
#endif