#pragma once
#ifndef GROUP
#define GROUP

#include "../Shared/DataInterfaces.h"

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr;

using std::pair;

//Forward Declarations
class Member;
class Template;

class Group : public ISave, public ILoad, public IExport, public IDelete, public IValidate
{
public:
	int Save() override;
	int Load() override;
	int Validate() override;
	int Delete() override;
	int Export() override;

private:
	string id_;
	string name_;
	//String: Member ID, Shared Pointer: Member pointer to be re-retrieved using the ID if memory location changes
	vector<pair<string, shared_ptr<Member>>> members_;
	//String: Template ID, Shared Pointer: Template pointer to be re-retrieved using the ID if memory location changes
	vector<pair<string, shared_ptr<Template>>> templates_;
};
#endif