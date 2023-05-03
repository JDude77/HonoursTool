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
class Template;

class TemplateField : public IDelete
{
public:
	TemplateField(Template* parentTemplate) : parentTemplate_(parentTemplate) {};
	bool operator==(const TemplateField&) const;
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
	ValidationRuleParameter* GetValidationRuleParameters(){ return &validationRuleParameters_; }

private:
	constexpr static int bufferMax_ = 64;

	//Note: ID referred to in-program as "name", but called ID internally to indicate that each field in a template must have a unique name
	char idBuffer_[bufferMax_] = {};
	int idBufferCurrentLength_ = 0;

	Template* parentTemplate_;
	string id_;
	DataType::DATA_TYPE dataType_ = DataType::DATA_TYPE::NONE;
	int typeInt_ = static_cast<int>(DataType::DATA_TYPE::NONE);
	vector<std::pair<RULES, int>> validationRules_;
	ValidationRuleParameter validationRuleParameters_;
};
#endif