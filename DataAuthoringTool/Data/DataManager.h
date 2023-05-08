#pragma once
#ifndef DATA_MANAGER
#define DATA_MANAGER

#include "Groups/Group.h"
#include "Members/Member.h"
#include "Templates/Template.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

class DataManager
{
	//String: Template's ID
	map<string, shared_ptr<Template>> templates_;
	int numberOfTemplates_ = 0;
	//String: Member's ID
	map<string, shared_ptr<Member>> members_;
	int numberOfMembers_ = 0;
	//String: Group's ID
	map<string, shared_ptr<Group>> groups_;
	int numberOfGroups_ = 0;
	inline static int internalIDCounter_ = -1;

	[[nodiscard]] int GetNumberOfTemplates()	const { return numberOfTemplates_; }
	[[nodiscard]] int GetNumberOfMembers()		const { return numberOfMembers_; }
	[[nodiscard]] int GetNumberOfGroups()		const { return numberOfGroups_; }

public:
	DataManager();

	vector<shared_ptr<Member>>		GetAllMembers();
	vector<shared_ptr<Template>>	GetAllTemplates();
	vector<shared_ptr<Group>>		GetAllGroups();

	shared_ptr<Template>	FindTemplateByInternalID(int internalID);
	shared_ptr<Member>		FindMemberByInternalID(int internalID);
	shared_ptr<Group>		FindGroupByInternalID(int internalID);
	
	void AddInstanceToDataMap(Member instance);
	void AddInstanceToDataMap(Template instance);
	void AddInstanceToDataMap(Group instance);

	void RemoveInstanceFromDataMap(Member instance);
	void RemoveInstanceFromDataMap(Template instance);
	void RemoveInstanceFromDataMap(Group instance);

	void UpdateInstance(const shared_ptr<Member>& instance);
	void UpdateInstance(const shared_ptr<Template>& instance);
	void UpdateInstance(const shared_ptr<Group>& instance);

	static int GetInternalID(bool incrementAfter);

	void DeleteInstanceAndCleanUpConnections(const shared_ptr<PrimaryData>& instance);
};
#endif