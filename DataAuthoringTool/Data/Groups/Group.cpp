#include "Group.h"

#include "../Members/Member.h"

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

int Group::Export()
{
	//TODO: Group Export functionality
	return 1;
}//End Export
