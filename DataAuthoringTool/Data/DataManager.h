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
class PrimaryData;
class Template;
class Member;
class Group;

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

	

	[[nodiscard]] int GetNumberOfTemplates() const { return numberOfTemplates_; }
	[[nodiscard]] int GetNumberOfMembers() const { return numberOfMembers_; }
	[[nodiscard]] int GetNumberOfGroups() const { return numberOfGroups_; }
public:
	vector<Member> GetAllMembers();
	vector<Template> GetAllTemplates();
	vector<Group> GetAllGroups();
	
	void AddInstanceToDataMap(PrimaryData instance);

	//To support user-defined IDs
	void UpdateInstanceIDInMap(PrimaryData instance);

	//void RemoveInstanceFromMapAndDelete(Instance instance);
};
#endif