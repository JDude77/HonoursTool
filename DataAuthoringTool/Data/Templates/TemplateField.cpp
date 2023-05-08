#include "TemplateField.h"

#include "Template.h"

bool TemplateField::operator==(const TemplateField& rhs) const
{
	const auto id = string(idBuffer_);
	const auto otherID = string(rhs.idBuffer_);
	return id == otherID && parentTemplate_ == rhs.parentTemplate_;
}//End == operator overload

int TemplateField::Delete()
{
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
	typeInt_ = static_cast<int>(type);
	if(dataType_ != cache) RefreshValidationRules(static_cast<int>(type));
}//End SetDataType