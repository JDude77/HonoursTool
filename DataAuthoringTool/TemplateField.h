#pragma once
#ifndef TEMPLATE_FIELD
#define TEMPLATE_FIELD

#include "DataInterfaces.h"
#include "DataTypes.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

//Forward Declarations
class ValidationRule;

class TemplateField : public IDelete
{
public:
	int Delete() override;

private:
	//Note: ID referred to in-program as "name", but called ID internally to indicate that each field in a template must have a unique name
	string id_;
	DataType dataType_ = None;
	vector<ValidationRule> validationRules_;
};
#endif