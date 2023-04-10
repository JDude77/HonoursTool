#include "Group.h"

#include "../Members/Member.h"
#include "../Templates/Template.h"

int Group::Save()
{
	//TODO: Group Save functionality
	return 1;
}//End Save

int Group::Load()
{
	//TODO: Group Load functionality
	return 1;
}//End Load

int Group::Validate()
{
	//TODO: Group Validate functionality extended
	for (auto& member : members_)
	{
		member.second->Validate();
	}//End for
	return 1;
}//End Validate

int Group::Delete()
{
	//TODO: Group Delete functionality
	return 1;
}//End Delete

int Group::Export(PrimaryData* caller)
{
	//TODO: Group Export functionality
	for(const auto& member : members_ | std::views::values)
	{
		member->Export(this);
	}//End for
	return 1;
}//End Export

vector<shared_ptr<Member>> Group::GetMembers()
{
	vector<shared_ptr<Member>> members;
	members.reserve(members_.size());
	for (auto& member : members_ | std::views::values)
	{
		members.push_back(member);
	}//End for
	return members;
}//End GetMembers

vector<shared_ptr<Template>> Group::GetTemplates()
{
	vector<shared_ptr<Template>> templates;
	templates.reserve(templates_.size());
	for (auto& temp : templates_ | std::views::values)
	{
		templates.push_back(temp);
	}//End for
	return templates;
}///End GetTemplates

int Group::AddTemplateToGroup(const shared_ptr<Template>& newTemplate)
{
	auto id = newTemplate->GetIDBuffer();
	const auto ptr = newTemplate.get();
	//Check to ensure the template isn't already in the group
	for (auto& [existingID, pointer] : templates_)
	{
		if(existingID == id || ptr == pointer.get())
		{
			return 0;
		}//End if
	}//End for

	//Add the template to the group
	templates_.emplace_back(id, newTemplate);
	return 1;
}//End AddTemplateToGroup

int Group::AddMemberToGroup(const shared_ptr<Member>& newMember)
{
	auto id = newMember->GetIDBuffer();
	const auto ptr = newMember.get();
	//Check to ensure the member isn't already in the group
	for (auto& [existingID, pointer] : members_)
	{
		if(existingID == id || ptr == pointer.get())
		{
			return 0;
		}//End if
	}//End for

	//Add the member to the group
	members_.emplace_back(id, newMember);
	return 1;
}//End AddMemberToGroup
