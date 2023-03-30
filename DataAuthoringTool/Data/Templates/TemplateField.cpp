#include "TemplateField.h"

#include "Template.h"

bool TemplateField::operator==(const TemplateField& rhs) const
{
	const auto comp = string(idBuffer_);
	const auto comp2 = string(rhs.idBuffer_);
	return comp == comp2 && parentTemplate_ == rhs.parentTemplate_;
}//End == operator overload

int TemplateField::Delete()
{
	//TODO: Template Field delete functionality
	//Clear data
	memset(idBuffer_, '\0', bufferMax_);
	idBufferCurrentLength_ = 0;

	//Remove self from parent template field list
	parentTemplate_->DeleteField(this);
	
	return 1;
}//End int

void TemplateField::RefreshValidationRules(const int type)
{
	validationRules_.clear();
	for (auto rule : ValidationRule::GetValidationRuleLabelsOfType(type))
	{
		validationRules_.emplace_back(rule, 0);
	}//End for
}//End RefreshValidationRules

void TemplateField::SetDataType(const int type)
{
	const int cache = typeInt_;
	dataType_ = static_cast<DataType::DATA_TYPE>(type);
	typeInt_ = type;
	if(typeInt_ != cache) RefreshValidationRules(type);
}//End SetDataType

void TemplateField::SetDataType(const DataType::DATA_TYPE type)
{
	const DataType::DATA_TYPE cache = dataType_;
	dataType_ = type;
	typeInt_ = type;
	if(dataType_ != cache) RefreshValidationRules(type);
}//End SetDataType