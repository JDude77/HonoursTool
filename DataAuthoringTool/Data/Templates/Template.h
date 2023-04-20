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
	int Export(PrimaryData* caller, std::shared_ptr<rapidjson::Document> jsonDocument) override;
	int Delete() override;
	bool IsEmpty() const override { return PrimaryData::IsEmpty() && fields_.empty(); }

	[[nodiscard]] int GetNumberOfFields() const { return fields_.size(); }
	TemplateField* GetFieldAtIndex(int index);
	[[nodiscard]] const vector<TemplateField> GetFields() const { return fields_; }
	int AddNewField();
	int DeleteField(const TemplateField* field);

private:
	vector<TemplateField> fields_;
};
#endif