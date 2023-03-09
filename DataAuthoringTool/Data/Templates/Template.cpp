#include "Template.h"

TemplateField* Template::GetFieldAtIndex(int index)
{
	return &fields_[index];
}//End GetFieldAtIndex

int Template::AddNewField()
{
	fields_.emplace_back();
	return 1;
}//End AddNewField