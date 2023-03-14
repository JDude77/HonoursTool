#pragma once
#ifndef MEMBER
#define MEMBER

#include "../Shared/DataInterfaces.h"
#include "MemberField.h"

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;

//Forward Declarations
class Template;
class MemberField;

class Member : public PrimaryData, public IValidate
{
public:
	Member(): PrimaryData(-1){}
	Member(const int internalID): PrimaryData(internalID){}
	int Save() override;
	int Load() override;
	int Export(PrimaryData* caller = nullptr) override;
	int Delete() override;
	int Validate() override;

	shared_ptr<Template> GetType() { return type_; }
	int GetTemplateIndex() const { return templateIndex_; }
	[[nodiscard]] int GetNumberOfFields() const { return fields_.size(); }
	[[nodiscard]] MemberField* GetFieldAtIndex(int index);
	void UpdateTemplateIndex(const shared_ptr<vector<Template>>& templates);
	void SetTemplateIndex(const int index) { templateIndex_ = index; }
	void SetType(const shared_ptr<Template>& temp);

private:
	int templateIndex_ = 0;
	shared_ptr<Template> type_ = nullptr;
	vector<MemberField> fields_;
};
#endif