#pragma once
#ifndef DATA_MANAGER
#define DATA_MANAGER

#include <map>
using std::map;
#include <string>
using std::string;

//Forward Declarations
class Template;
class Member;
class Group;

class DataManager
{
	//String: Template's ID
	map<string, Template> templates_;
	//String: Member's ID
	map<string, Member> members_;
	//String: Group's ID
	map<string, Group> groups_;

	template <class Instance>
	void AddInstanceToMap(Instance instance);

	//To support user-defined IDs
	template <class Instance>
	void UpdateInstanceIDInMap(Instance instance);

	template <class Instance>
	void RemoveInstanceFromMapAndDelete(Instance instance);
};
#endif