#pragma once
#ifndef MEMBER
#define MEMBER

#include "../Shared/DataInterfaces.h"

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
	int Save() override;
	int Load() override;
	int Export() override;
	int Delete() override;
	int Validate() override;

	shared_ptr<Template> GetType() { return type_; }

private:
	shared_ptr<Template> type_ = nullptr;
	vector<MemberField> fields_;
};
#endif