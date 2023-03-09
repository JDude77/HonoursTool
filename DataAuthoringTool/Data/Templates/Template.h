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
	Template(): PrimaryData(-1){}
	Template(const int internalID): PrimaryData(internalID){}
	int Save() override{return 1;}
	int Load() override{return 1;};
	int Export() override{return 1;};
	int Delete() override{return 1;};

	[[nodiscard]] int GetNumberOfFields() const { return fields_.size(); }
	TemplateField* GetFieldAtIndex(int index);
	int AddNewField();
	
private:
	vector<TemplateField> fields_;
};
#endif