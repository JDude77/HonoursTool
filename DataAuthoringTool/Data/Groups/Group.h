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

class Group : public PrimaryData, public IValidate
{
public:
	Group() : PrimaryData(-1){}
	Group(const int internalID) : PrimaryData(internalID){}
	int Save() override;
	int Load() override;
	int Validate(std::queue<std::string>* outputText) override;
	int Delete() override;
	int Export(std::queue<std::string>* outputText, PrimaryData* caller = nullptr) override;
	int Export(std::queue<std::string>* outputText, PrimaryData* caller, std::shared_ptr<rapidjson::Document> jsonDocument) override;
	[[nodiscard]] bool IsEmpty() const override {  return PrimaryData::IsEmpty() && members_.empty() && templates_.empty(); }

	[[nodiscard]] int GetNumberOfTemplates() const { return templates_.size(); }
	[[nodiscard]] int GetNumberOfMembers() const { return members_.size(); }
	vector<shared_ptr<Member>> GetMembers();
	vector<shared_ptr<Template>> GetTemplates();
	int* GetActiveTemplateTab(){ return &activeTemplateTab_; }
	void SetActiveTemplateTab(const int tabNumber){ activeTemplateTab_ = tabNumber; }

	int AddTemplateToGroup(const shared_ptr<Template>& newTemplate);
	int AddMemberToGroup(const shared_ptr<Member>& newMember);

	int RemoveTemplateFromGroup(const shared_ptr<Template>& templateToRemove);
	int RemoveMemberFromGroup(const shared_ptr<Member>& memberToRemove);

private:
	//String: Member ID, Shared Pointer: Member pointer to be re-retrieved using the ID if memory location changes
	vector<pair<string, shared_ptr<Member>>> members_;
	//String: Template ID, Shared Pointer: Template pointer to be re-retrieved using the ID if memory location changes
	vector<pair<string, shared_ptr<Template>>> templates_;
	//Which tab was last active
	int activeTemplateTab_ = -1;
};
#endif