#pragma once
#ifndef MEMBER
#define MEMBER

#include "../Shared/DataInterfaces.h"
#include "MemberField.h"
#include <string>
#include <vector>
#include <memory>
using std::string;
using std::vector;
using std::shared_ptr;

//Forward Declarations
class Template;

class Member : public PrimaryData, public IValidate
{
public:
	Member(): PrimaryData(-1){}
	Member(const int internalID): PrimaryData(internalID){}
	int Save() override;
	int Load() override;
	int Export(std::queue<std::string>* outputText, PrimaryData* caller = nullptr) override;
	int Export(std::queue<std::string>* outputText, PrimaryData* caller, std::shared_ptr<Document> jsonDocument) override;
	int Delete() override;
	int Validate(std::queue<std::string>* outputText) override;
	[[nodiscard]] bool IsEmpty() const override;

	[[nodiscard]] int GetTemplateIndex() const { return templateIndex_; }
	[[nodiscard]] int GetNumberOfFields() const { return fields_.size(); }
	[[nodiscard]] MemberField* GetFieldAtIndex(int index);

	void UpdateTemplateIndex(const shared_ptr<vector<Template>>& templates);
	void SetTemplateIndex(const int index) { templateIndex_ = index; }

	shared_ptr<Template> GetType() { return type_; }
	void SetType(const shared_ptr<Template>& temp);

	void RefreshFieldName(int fieldIndex);
	void RefreshFieldType(int fieldIndex);
	void RefreshFieldQuantity();

private:
	int templateIndex_ = 0;
	shared_ptr<Template> type_ = nullptr;
	vector<MemberField> fields_;
};
#endif