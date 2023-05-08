#pragma once
#ifndef TEMPLATE
#define TEMPLATE

#include "../Shared/DataInterfaces.h"
#include "TemplateField.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

class Template : public PrimaryData
{
public:
	Template(): PrimaryData(-1){}
	Template(const char* ID, int internalID);
	Template(const int internalID): PrimaryData(internalID){}
	int Save() override;
	int Load() override;
	int Export(std::queue<std::string>* outputText, PrimaryData* caller = nullptr) override;
	int Export(std::queue<std::string>* outputText, PrimaryData* caller, std::shared_ptr<Document> jsonDocument) override;
	int Delete() override;
	[[nodiscard]] bool IsEmpty() const override { return PrimaryData::IsEmpty() && fields_.empty(); }

	[[nodiscard]] int GetNumberOfFields() const { return fields_.size(); }
	TemplateField* GetFieldAtIndex(int index);
	[[nodiscard]] vector<TemplateField> GetFields() const { return fields_; }
	int AddNewField();
	int DeleteField(const TemplateField* field);

private:
	vector<TemplateField> fields_;
};
#endif