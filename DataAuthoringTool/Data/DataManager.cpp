#include "DataManager.h"

#include <ranges>

#include "Shared/DataInterfaces.h"
#include "Members/Member.h"
#include "Templates/Template.h"
#include "Groups/Group.h"

using std::reinterpret_pointer_cast;

vector<PrimaryData> DataManager::GetAllMembers()
{
	vector<PrimaryData> members;
	for (const auto& member : members_ | std::views::values)
	{
		members.push_back(member);
	}//End for

	return members;
}//End GetAllMembers

vector<PrimaryData> DataManager::GetAllTemplates()
{
	vector<PrimaryData> templates;
	for (const auto& temp : templates_ | std::views::values)
	{
		templates.push_back(temp);
	}//End for

	return templates;
}//End GetAllTemplates

vector<PrimaryData> DataManager::GetAllGroups()
{
	vector<PrimaryData> groups;
	for (const auto& group : groups_ | std::views::values)
	{
		groups.push_back(group);
	}//End for

	return groups;
}//End GetAllGroups

vector<PrimaryData> DataManager::GetAllInstancesOfType(const shared_ptr<PrimaryData>& type)
{
	if(shared_ptr<Member> member = reinterpret_pointer_cast<Member>(type))
	{
		return GetAllMembers();
	}//End if

	if(shared_ptr<Template> temp = reinterpret_pointer_cast<Template>(type))
	{
		return GetAllTemplates();
	}//End if

	if(shared_ptr<Group> group = reinterpret_pointer_cast<Group>(type))
	{
		return GetAllGroups();
	}//End if

	return {};
}//End GetAllInstanceOfType