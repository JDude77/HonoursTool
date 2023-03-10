#include "Member.h"

#include "../Templates/Template.h"

int Member::Save()
{
	//TODO: Member save functionality
	return 1;
}//End Save

int Member::Load()
{
	//TODO: Member load functionality
	return 1;
}//End Load

int Member::Export()
{
	//TODO: Member export functionality
	return 1;
}//End Export

int Member::Delete()
{
	//TODO: Member delete functionality
	return 1;
}//End Delete

int Member::Validate()
{
	//TODO: Member validate functionality
	return 1;
}//End Validate

MemberField* Member::GetFieldAtIndex(const int index)
{
	return &fields_[index];
}//End GetFieldAtIndex

void Member::SetType(const shared_ptr<Template>& temp)
{
	type_ = temp;

	if(!fields_.empty()) fields_.clear();

	for(const auto fields = type_->GetFields(); TemplateField field : fields)
	{
		fields_.emplace_back(this, *field.GetDataType());
	}//End for
}//End SetType