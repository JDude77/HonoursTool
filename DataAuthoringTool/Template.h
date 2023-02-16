#pragma once
#ifndef TEMPLATE
#define TEMPLATE

#include "DataInterfaces.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

//Forward Declarations
class ValidationRule;
class TemplateField;

class Template : public ISave, public ILoad, public IExport, public IDelete
{
public:
	int Save() override;
	int Load() override;
	int Export() override;
	int Delete() override;

private:
	string name_;
	string id_;
	vector<TemplateField> fields_;
};
#endif