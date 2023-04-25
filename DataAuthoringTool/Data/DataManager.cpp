#include "DataManager.h"

#include <ranges>

DataManager::DataManager()
{
	//Push default empty template as "none" value for member selection
	AddInstanceToDataMap(Template("None", GetInternalID(true)));
}//End constructor

vector<shared_ptr<Member>> DataManager::GetAllMembers()
{
	vector<shared_ptr<Member>> members;
	for (const auto& member : members_ | std::views::values)
	{
		members.push_back(member);
	}//End for

	return members;
}//End GetAllMembers

vector<shared_ptr<Template>> DataManager::GetAllTemplates()
{
	vector<shared_ptr<Template>> templates;
	for (const auto& temp : templates_ | std::views::values)
	{
		templates.push_back(temp);
	}//End for

	//Push the empty "NONE" template to the first slot
	for (auto it = templates.begin(); it != templates.end(); ++it)
	{
		if(it->get()->GetInternalID() == -1)
		{
			std::ranges::rotate(it, it + 1, templates.end());
			break;
		}//End if
	}//End for

	return templates;
}//End GetAllTemplates

vector<shared_ptr<Group>> DataManager::GetAllGroups()
{
	vector<shared_ptr<Group>> groups;
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
	members_.insert({id, std::make_shared<Member>(instance)});
	++numberOfMembers_;
}//End AddInstanceToDataMap (Member)

void DataManager::AddInstanceToDataMap(Template instance)
{
	string id;
	id.append(instance.GetIDBuffer());
	templates_.insert({id, std::make_shared<Template>(instance)});
	++numberOfTemplates_;
}//End AddInstanceToDataMap (Template)

void DataManager::AddInstanceToDataMap(Group instance)
{
	string id;
	id.append(instance.GetIDBuffer());
	groups_.insert({id, std::make_shared<Group>(instance)});
	++numberOfGroups_;
}//End AddInstanceToDataMap (Group)

void DataManager::RemoveInstanceFromDataMap(Member instance)
{
	const auto iterator = members_.find(instance.GetIDBuffer());
	if(iterator == members_.end()) return;

	members_.erase(iterator);
}//End RemoveInstanceFromDataMap (Member)

void DataManager::RemoveInstanceFromDataMap(Template instance)
{
	const auto iterator = templates_.find(instance.GetIDBuffer());
	if(iterator == templates_.end()) return;

	templates_.erase(iterator);
}//End RemoveInstanceFromDataMap (Template)

void DataManager::RemoveInstanceFromDataMap(Group instance)
{
	const auto iterator = groups_.find(instance.GetIDBuffer());
	if(iterator == groups_.end()) return;

	groups_.erase(iterator);
}//End RemoveInstanceFromDataMap (Group)

void DataManager::UpdateInstance(const shared_ptr<Member>& instance)
{
	if(members_.contains(instance->GetIDBuffer()))
	{
		members_[instance->GetIDBuffer()] = instance;
	}//End if
	else
	{
		for (auto i = members_.begin(); i != members_.end(); ++i)
		{
			if(i->second->GetInternalID() == instance->GetInternalID())
			{
				members_.erase(i);
				break;
			}//End if
		}//End for
		members_.insert({instance->GetIDBuffer(), instance});
	}//End else
}//End UpdateInstance (Member)

void DataManager::UpdateInstance(const shared_ptr<Template>& instance)
{
	if(templates_.contains(instance->GetIDBuffer()))
	{
		templates_[instance->GetIDBuffer()] = instance;
	}//End if
	else
	{
		for (auto i = templates_.begin(); i != templates_.end(); ++i)
		{
			if(i->second->GetInternalID() == instance->GetInternalID())
			{
				templates_.erase(i);
				break;
			}//End if
		}//End for
		templates_.insert({instance->GetIDBuffer(), instance});
	}//End else
}//End UpdateInstance (Template)

void DataManager::UpdateInstance(const shared_ptr<Group>& instance)
{
	if(groups_.contains(instance->GetIDBuffer()))
	{
		groups_[instance->GetIDBuffer()] = instance;
	}//End if
	else
	{
		for (auto i = groups_.begin(); i != groups_.end(); ++i)
		{
			if(i->second->GetInternalID() == instance->GetInternalID())
			{
				groups_.erase(i);
				break;
			}//End if
		}//End for
		groups_.insert({instance->GetIDBuffer(), instance});
	}//End else
}//End UpdateInstance (Group)

int DataManager::GetInternalID(const bool incrementAfter)
{
	if(!incrementAfter) return internalIDCounter_;

	const int temp = internalIDCounter_;
	++internalIDCounter_;
	return temp;
}//End GetInternalID