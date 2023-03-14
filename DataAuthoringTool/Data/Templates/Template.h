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
	Template(const char* ID, const int internalID);
	Template(const int internalID): PrimaryData(internalID){}
	int Save() override;
	int Load() override;
	int Export(PrimaryData* caller = nullptr) override;
	int Delete() override;

	[[nodiscard]] int GetNumberOfFields() const { return fields_.size(); }
	TemplateField* GetFieldAtIndex(int index);
	[[nodiscard]] const vector<TemplateField> GetFields() const { return fields_; }
	int AddNewField();
	
private:
	vector<TemplateField> fields_;
};
#endif