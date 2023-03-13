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

void Member::UpdateTemplateIndex(const shared_ptr<vector<Template>>& templates)
{
	//If this passes, index doesn't need updated
	if(&templates->at(templateIndex_) == type_.get()) return;

	//Find new index
	for(int i = 0; i < templates->size(); i++)
	{
		if(&templates->at(i) == type_.get())
		{
			templateIndex_ = i;
			return;
		}//End if
	}//End for
}//End UpdateTemplateIndex

void Member::SetType(const shared_ptr<Template>& temp)
{
	type_ = temp;

	if(!fields_.empty()) fields_.clear();

	const auto fields = type_->GetFields();

	for(int i = 0; i < type_->GetNumberOfFields(); i++)
	{
		TemplateField field = fields[i];
		fields_.emplace_back(this, *field.GetDataType(), i);
	}//End for
}//End SetType