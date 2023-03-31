#include "Member.h"

#include "../Templates/Template.h"
#include "../../RapidJSON/document.h"
#include "../../RapidJSON/filewritestream.h"
#include "../../RapidJSON/writer.h"
using namespace rapidjson;

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

int Member::Export(PrimaryData* caller)
{
	//TODO: Member export functionality
	//Create JSON Document
	Document jsonDocument;
	jsonDocument.SetObject();
	Document::AllocatorType& allocator = jsonDocument.GetAllocator();

	//Loop through every member field
	for (const MemberField& field : fields_)
	{
		auto name = GenericValue<UTF8<>>::StringRefType(field.GetName());
		//Correctly grab the data in a JSON value for each type
		switch(field.GetDataType())
		{
			case DataType::STRING:
				{
					Value stringVal;
					stringVal.SetString(GenericValue<UTF8<>>::StringRefType(field.GetDataBuffer()));
					jsonDocument.AddMember(name, stringVal, allocator);
				}//End String data handling
				break;

			case DataType::INTEGER:
				{
					Value integerVal;
					int data = strtol(field.GetDataBuffer(), nullptr, 0);
					integerVal.SetInt(data);
					jsonDocument.AddMember(name, integerVal, allocator);
				}//End Integer data handling
				break;

			case DataType::FLOAT:
				{
					Value floatVal;
					float data = strtof(field.GetDataBuffer(), nullptr);
					floatVal.SetFloat(data);
					jsonDocument.AddMember(name, floatVal, allocator);
				}//End Float data handling
				break;

			case DataType::CHAR:
				{
					Value charVal;
					charVal.SetString(GenericValue<UTF8<>>::StringRefType(field.GetDataBuffer()));
					jsonDocument.AddMember(name, charVal, allocator);
				}//End Char data handling
				break;

			case DataType::BOOLEAN:
				{
					Value booleanVal;
					booleanVal.SetBool(field.GetBooleanData());
					jsonDocument.AddMember(name, booleanVal, allocator);
				}//End Boolean data handling
				break;
			default: break;
		}//End switch
	}//End for
	{
		//Construct file name
		string fileName;
		fileName.append(idBuffer_);
		fileName.append(".json");

		//Set up buffer/stream/file
		FILE* filePath = fopen(fileName.c_str(), "wb");
		char writeBuffer[65536];
		FileWriteStream stream(filePath, writeBuffer, sizeof writeBuffer);

		//Write to file
		Writer writer(stream);
		jsonDocument.Accept(writer);

		//Close file
		return fclose(filePath);
	}//End Exporting functionality
}//End Export

int Member::Delete()
{
	//TODO: Member delete functionality
	return 1;
}//End Delete

int Member::Validate()
{
	for (auto& field : fields_)
	{
		field.Validate();
	}//End for
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