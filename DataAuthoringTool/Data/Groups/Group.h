#pragma once
#ifndef GROUP
#define GROUP

#include <ranges>

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

class Group : public PrimaryData, public IValidate
{
public:
	Group() : PrimaryData(-1){}
	Group(const int internalID) : PrimaryData(internalID){}
	int Save() override;
	int Load() override;
	int Validate() override;
	int Delete() override;
	int Export(PrimaryData* caller = nullptr) override;
	[[nodiscard]] bool IsEmpty() const override {  return PrimaryData::IsEmpty() && members_.empty() && templates_.empty(); }

	[[nodiscard]] int GetNumberOfTemplates() const { return templates_.size(); }
	[[nodiscard]] int GetNumberOfMembers() const { return members_.size(); }
	vector<shared_ptr<Member>> GetMembers();
	vector<shared_ptr<Template>> GetTemplates();

	int AddTemplateToGroup(const shared_ptr<Template>& newTemplate);
	int AddMemberToGroup(const shared_ptr<Member>& newMember);

private:
	//String: Member ID, Shared Pointer: Member pointer to be re-retrieved using the ID if memory location changes
	vector<pair<string, shared_ptr<Member>>> members_;
	//String: Template ID, Shared Pointer: Template pointer to be re-retrieved using the ID if memory location changes
	vector<pair<string, shared_ptr<Template>>> templates_;
};
#endif