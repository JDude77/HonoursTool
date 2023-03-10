#include "Template.h"

Template::Template(const char* ID, const int internalID) : PrimaryData(internalID)
{
	for (int i = 0 ; i < sizeof(ID); i++)
	{
		idBuffer_[i] = ID[i];
		nameBuffer_[i] = ID[i];
	}//End for
}//End Template constructor

TemplateField* Template::GetFieldAtIndex(int index)
{
	return &fields_[index];
}//End GetFieldAtIndex

int Template::AddNewField()
{
	fields_.emplace_back();
	return 1;
}//End AddNewField