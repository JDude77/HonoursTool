#pragma once
#ifndef TEMPLATE
#define TEMPLATE

#include "../Shared/DataInterfaces.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

//Forward Declarations
class ValidationRule;
class TemplateField;

class Template : public PrimaryData
{
public:
	int Save() override;
	int Load() override;
	int Export() override;
	int Delete() override;

private:
	vector<TemplateField> fields_;
};
#endif