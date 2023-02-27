#pragma once
#ifndef TEMPLATE
#define TEMPLATE

#include "../Shared/DataInterfaces.h"
#include "TemplateField.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

//Forward Declarations
//class ValidationRule;

class Template : public PrimaryData
{
public:
	Template() = default;
	int Save() override{return 1;};
	int Load() override{return 1;};
	int Export() override{return 1;};
	int Delete() override{return 1;};

private:
	vector<TemplateField> fields_;
};
#endif