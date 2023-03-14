#include "Template.h"

Template::Template(const char* ID, const int internalID) : PrimaryData(internalID)
{
	for (int i = 0 ; i < sizeof(ID); i++)
	{
		idBuffer_[i] = ID[i];
		nameBuffer_[i] = ID[i];
	}//End for
}//End Template constructor

int Template::Save()
{
	//TODO: Template Save funtionality
	return 1;
}//End Save (Template)

int Template::Load()
{
	//TODO: Template Load functionality
	return 1;
}//End Load (Template)

int Template::Export(PrimaryData* caller)
{
	//TODO: Template Export functionality
	return 1;
}//End Export (Template)

int Template::Delete()
{
	//TODO: Template Delete functionality
	return 1;
}//End Delete (Template)

TemplateField* Template::GetFieldAtIndex(const int index)
{
	return &fields_[index];
}//End GetFieldAtIndex

int Template::AddNewField()
{
	fields_.emplace_back();
	return 1;
}//End AddNewField