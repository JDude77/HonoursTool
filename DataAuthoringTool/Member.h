#pragma once
#ifndef MEMBER
#define MEMBER

#include "DataInterfaces.h"

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;

//Forward Declarations
class Template;
class MemberField;

class Member : public ISave, public ILoad, public IExport, public IDelete, public IValidate
{
public:
	int Save() override;
	int Load() override;
	int Export() override;
	int Delete() override;
	int Validate() override;

private:
	string id_;
	string name_;
	shared_ptr<Template> type_ = nullptr;
	vector<MemberField> fields_;
};
#endif