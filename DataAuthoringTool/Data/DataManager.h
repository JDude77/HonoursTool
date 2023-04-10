#pragma once
#ifndef DATA_MANAGER
#define DATA_MANAGER

#include <map>
using std::map;
#include <memory>
using std::shared_ptr;
#include <string>
using std::string;
#include <vector>
using std::vector;

//Forward Declarations
#include "Members/Member.h"
#include "Templates/Template.h"
#include "Groups/Group.h"

class DataManager
{
	//String: Template's ID
	map<string, Template> templates_;
	int numberOfTemplates_ = 0;
	//String: Member's ID
	map<string, Member> members_;
	int numberOfMembers_ = 0;
	//String: Group's ID
	map<string, Group> groups_;
	int numberOfGroups_ = 0;
	inline static int internalIDCounter_ = -1;

	[[nodiscard]] int GetNumberOfTemplates() const { return numberOfTemplates_; }
	[[nodiscard]] int GetNumberOfMembers() const { return numberOfMembers_; }
	[[nodiscard]] int GetNumberOfGroups() const { return numberOfGroups_; }
public:
	DataManager();

	vector<Member> GetAllMembers();
	vector<Template> GetAllTemplates();
	vector<Group> GetAllGroups();
	
	void AddInstanceToDataMap(Member instance);
	void AddInstanceToDataMap(Template instance);
	void AddInstanceToDataMap(Group instance);

	void RemoveInstanceFromDataMap(Member instance);
	void RemoveInstanceFromDataMap(Template instance);
	void RemoveInstanceFromDataMap(Group instance);

	void UpdateInstance(Member* instance);
	void UpdateInstance(Template* instance);
	void UpdateInstance(Group* instance);

	static int GetInternalID(bool incrementAfter);
};
#endif