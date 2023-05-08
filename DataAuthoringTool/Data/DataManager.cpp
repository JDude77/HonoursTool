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
	for (const shared_ptr<Member>& member : members_ | std::views::values)
	{
		members.push_back(member);
	}//End for

	return members;
}//End GetAllMembers

vector<shared_ptr<Template>> DataManager::GetAllTemplates()
{
	vector<shared_ptr<Template>> templates;
	for (const shared_ptr<Template>& temp : templates_ | std::views::values)
	{
		templates.push_back(temp);
	}//End for

	//Push the empty "NONE" template to the first slot
	for (auto it = templates.begin(); it != templates.end(); ++it)
	{
		if(it->get()->GetInternalID() == -1)
		{
			std::ranges::rotate(templates, it);
			break;
		}//End if
	}//End for

	return templates;
}//End GetAllTemplates

vector<shared_ptr<Group>> DataManager::GetAllGroups()
{
	vector<shared_ptr<Group>> groups;
	for (const shared_ptr<Group>& group : groups_ | std::views::values)
	{
		groups.push_back(group);
	}//End for

	return groups;
}//End GetAllGroups

shared_ptr<Template> DataManager::FindTemplateByInternalID(const int internalID)
{
	for(const shared_ptr<Template>& temp : templates_ | std::views::values)
	{
		if(temp->GetInternalID() == internalID) return temp;
	}//End for
	return nullptr;
}//End FindTemplateByInternalID

shared_ptr<Member> DataManager::FindMemberByInternalID(const int internalID)
{
	for(const shared_ptr<Member>& member : members_ | std::views::values)
	{
		if(member->GetInternalID() == internalID) return member;
	}//End for
	return nullptr;
}//End FindMemberByInternalID

shared_ptr<Group> DataManager::FindGroupByInternalID(const int internalID)
{
	for(const shared_ptr<Group>& group : groups_ | std::views::values)
	{
		if(group->GetInternalID() == internalID) return group;
	}//End for
	return nullptr;
}//End FindGroupByInternalID

void DataManager::AddInstanceToDataMap(Member instance)
{
	string id = instance.GetIDBuffer();
	members_.insert({id, std::make_shared<Member>(instance)});
	++numberOfMembers_;
}//End AddInstanceToDataMap (Member)

void DataManager::AddInstanceToDataMap(Template instance)
{
	string id = instance.GetIDBuffer();
	templates_.insert({id, std::make_shared<Template>(instance)});
	++numberOfTemplates_;
}//End AddInstanceToDataMap (Template)

void DataManager::AddInstanceToDataMap(Group instance)
{
	string id = instance.GetIDBuffer();
	groups_.insert({id, std::make_shared<Group>(instance)});
	++numberOfGroups_;
}//End AddInstanceToDataMap (Group)

void DataManager::RemoveInstanceFromDataMap(Member instance)
{
	const auto iterator = members_.find(instance.GetIDBuffer());
	if(iterator == members_.end()) return;

	members_.erase(iterator);
	--numberOfMembers_;
}//End RemoveInstanceFromDataMap (Member)

void DataManager::RemoveInstanceFromDataMap(Template instance)
{
	const auto iterator = templates_.find(instance.GetIDBuffer());
	if(iterator == templates_.end()) return;

	templates_.erase(iterator);
	--numberOfTemplates_;
}//End RemoveInstanceFromDataMap (Template)

void DataManager::RemoveInstanceFromDataMap(Group instance)
{
	const auto iterator = groups_.find(instance.GetIDBuffer());
	if(iterator == groups_.end()) return;

	groups_.erase(iterator);
	--numberOfGroups_;
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

void DataManager::DeleteInstanceAndCleanUpConnections(const shared_ptr<PrimaryData>& instance)
{
	if(const shared_ptr<Member> member = std::dynamic_pointer_cast<Member>(instance))
	{
		//Remove member from any groups it's in
		for (auto it = groups_.begin(); it != groups_.end(); ++it)
		{
			const shared_ptr<Group> cachedGroup = it->second;
			cachedGroup->RemoveMemberFromGroup(member);
		}//End for

		//Set its type to None
		member->SetType(templates_["None"]);
		member->SetTemplateIndex(0);
	}//End if

	else if(const shared_ptr<Template> temp = std::dynamic_pointer_cast<Template>(instance))
	{
		//Check all groups for this template type
		for(auto it = groups_.begin(); it != groups_.end(); ++it)
		{
			const auto cachedGroup = it->second;
			cachedGroup->RemoveTemplateFromGroup(temp);
		}//End for

		//Set type of template back to none for all members of this type
		for(auto it = members_.begin(); it != members_.end(); ++it)
		{
			if(const shared_ptr<Member> cachedMember = it->second; cachedMember->GetType()->GetInternalID() == temp->GetInternalID())
			{
				cachedMember->SetType(templates_["None"]);
				cachedMember->SetTemplateIndex(0);
			}//End if
		}//End for
	}//End else if
}//End DeleteInstanceAndCleanUpConnections
