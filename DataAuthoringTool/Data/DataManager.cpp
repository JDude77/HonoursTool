#include "DataManager.h"

#include <ranges>

using std::reinterpret_pointer_cast;

vector<Member> DataManager::GetAllMembers()
{
	vector<Member> members;
	for (const auto& member : members_ | std::views::values)
	{
		members.push_back(member);
	}//End for

	return members;
}//End GetAllMembers

vector<Template> DataManager::GetAllTemplates()
{
	vector<Template> templates;
	for (const auto& temp : templates_ | std::views::values)
	{
		templates.push_back(temp);
	}//End for

	return templates;
}//End GetAllTemplates

vector<Group> DataManager::GetAllGroups()
{
	vector<Group> groups;
	for (const auto& group : groups_ | std::views::values)
	{
		groups.push_back(group);
	}//End for

	return groups;
}//End GetAllGroups

void DataManager::AddInstanceToDataMap(Member instance)
{
	string id;
	id.append(instance.GetIDBuffer());
	members_.insert({id, instance});
}//End AddInstanceToDataMap

void DataManager::AddInstanceToDataMap(Template instance)
{
	string id;
	id.append(instance.GetIDBuffer());
	templates_.insert({id, instance});
}//End AddInstanceToDataMap

void DataManager::AddInstanceToDataMap(Group instance)
{
	string id;
	id.append(instance.GetIDBuffer());
	groups_.insert({id, instance});
}//End AddInstanceToDataMap
