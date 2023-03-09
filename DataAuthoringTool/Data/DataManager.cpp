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
	++numberOfMembers_;
}//End AddInstanceToDataMap

void DataManager::AddInstanceToDataMap(Template instance)
{
	string id;
	id.append(instance.GetIDBuffer());
	templates_.insert({id, instance});
	++numberOfTemplates_;
}//End AddInstanceToDataMap

void DataManager::AddInstanceToDataMap(Group instance)
{
	string id;
	id.append(instance.GetIDBuffer());
	groups_.insert({id, instance});
	++numberOfGroups_;
}//End AddInstanceToDataMap

void DataManager::UpdateMember(Member* member)
{
	if(members_.contains(member->GetIDBuffer()))
	{
		members_[member->GetIDBuffer()] = *member;
	}//End if
	else
	{
		for (auto i = members_.begin(); i != members_.end(); ++i)
		{
			if(i->second.GetInternalID() == member->GetInternalID())
			{
				members_.erase(i);
				break;
			}//End if
		}//End for
		members_.insert({member->GetIDBuffer(), *member});
	}//End else
}//End Update Member

void DataManager::UpdateGroup(Group* group)
{
	if(groups_.contains(group->GetIDBuffer()))
	{
		groups_[group->GetIDBuffer()] = *group;
	}//End if
	else
	{
		for (auto i = groups_.begin(); i != groups_.end(); ++i)
		{
			if(i->second.GetInternalID() == group->GetInternalID())
			{
				groups_.erase(i);
				break;
			}//End if
		}//End for
		groups_.insert({group->GetIDBuffer(), *group});
	}//End else
}//End Update Group

int DataManager::GetInternalID(const bool incrementAfter)
{
	if(!incrementAfter) return internalIDCounter_;

	const int temp = internalIDCounter_;
	++internalIDCounter_;
	return temp;
}//End GetInternalID

void DataManager::UpdateTemplate(Template* temp)
{
	if(templates_.contains(temp->GetIDBuffer()))
	{
		templates_[temp->GetIDBuffer()] = *temp;
	}//End if
	else
	{
		for (auto i = templates_.begin(); i != templates_.end(); ++i)
		{
			if(i->second.GetInternalID() == temp->GetInternalID())
			{
				templates_.erase(i);
				break;
			}//End if
		}//End for
		templates_.insert({temp->GetIDBuffer(), *temp});
	}//End else
}//End Update Template