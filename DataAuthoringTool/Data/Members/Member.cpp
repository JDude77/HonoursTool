#include "Member.h"

#include <iostream>

#include "../Templates/Template.h"
#include "../../RapidJSON/filewritestream.h"
#include "../../RapidJSON/prettywriter.h"
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

	//Get member name and put it in the doc
	Value nameValue;
	nameValue.SetString(GenericValue<UTF8<>>::StringRefType(GetNameBuffer()));
	jsonDocument.AddMember("Name", nameValue, allocator);

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
		PrettyWriter writer(stream);
		jsonDocument.Accept(writer);

		//Close file
		return fclose(filePath);
	}//End Exporting functionality
}//End Export

//Export call for adding a member to a group JSON file
int Member::Export(PrimaryData* caller, std::shared_ptr<rapidjson::Document> jsonDocument)
{
	//Get reference to the allocator for adding new objects
	Document::AllocatorType& allocator = jsonDocument->GetAllocator();

	//Create potential new array value here to prevent premature garbage collection
	Value memberArray(kArrayType);

	//Check to see if a member of this type already exists in the document
	if(jsonDocument->HasMember(GetType()->GetNameBuffer()))
	{
		//Create array of members of this type
		const Value::MemberIterator it = jsonDocument->FindMember(GetType()->GetNameBuffer());
		memberArray = it->value;
		jsonDocument->RemoveMember(GetType()->GetNameBuffer());
	}//End if

	//Construct JSON object of member
	Value memberObject(kObjectType);

	Value nameValue;
	nameValue.SetString(GenericValue<UTF8<>>::StringRefType(GetNameBuffer()));
	memberObject.AddMember("Name", nameValue, allocator);

	Value fieldArray(kArrayType);
	
	for (const MemberField& field : fields_)
	{
		auto name = GenericValue<UTF8<>>::StringRefType(field.GetName());

		Value fieldValue(kObjectType);

		//Correctly grab the data in a JSON value for each type
		switch(field.GetDataType())
		{
			case DataType::STRING:
			{
				Value stringVal;
				stringVal.SetString(GenericValue<UTF8<>>::StringRefType(field.GetDataBuffer()));
				fieldValue.AddMember(name, stringVal, allocator);
			}//End String data handling
			break;

			case DataType::INTEGER:
			{
				Value integerVal;
				int data = strtol(field.GetDataBuffer(), nullptr, 0);
				integerVal.SetInt(data);
				fieldValue.AddMember(name, integerVal, allocator);
			}//End Integer data handling
			break;

			case DataType::FLOAT:
			{
				Value floatVal;
				float data = strtof(field.GetDataBuffer(), nullptr);
				floatVal.SetFloat(data);
				fieldValue.AddMember(name, floatVal, allocator);
			}//End Float data handling
			break;

			case DataType::CHAR:
			{
				Value charVal;
				charVal.SetString(GenericValue<UTF8<>>::StringRefType(field.GetDataBuffer()));
				fieldValue.AddMember(name, charVal, allocator);
			}//End Char data handling
			break;

			case DataType::BOOLEAN:
			{
				Value booleanVal;
				auto check = *field.GetBooleanData();
				booleanVal.SetBool(check);
				fieldValue.AddMember(name, booleanVal, allocator);
			}//End Boolean data handling
			break;

			default: break;
		}//End switch

		//Add field into field array
		fieldArray.PushBack(fieldValue, allocator);
	}//End for

	//Add constructed field array to member object
	memberObject.AddMember("Fields", fieldArray, allocator);

	//Add this member on to the array
	memberArray.PushBack(memberObject, allocator);

	//Add the array back into the JSON document
	jsonDocument->AddMember(GenericValue<UTF8<>>::StringRefType(GetType()->GetNameBuffer()), memberArray, allocator);

	return jsonDocument->HasMember(GetType()->GetNameBuffer());
}//End Export

int Member::Delete()
{
	//TODO: Member delete functionality
	return 1;
}//End Delete

int Member::Validate()
{
	std::cout << "Member " << idBuffer_ << " Validating Data..." << std::endl;
	for (auto& field : fields_)
	{
		field.Validate();
	}//End for
	std::cout << "Member " << idBuffer_ << " Validation Complete" << std::endl;
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

void Member::RefreshFieldName(const int fieldIndex)
{
	if(fields_.empty()) return;
	fields_.at(fieldIndex).RefreshName();
}//End RefreshFieldName

void Member::RefreshFieldType(const int fieldIndex)
{
	if(fields_.empty()) return;
	fields_.at(fieldIndex).RefreshType();
}//End RefreshFieldType

void Member::RefreshFieldQuantity()
{
	vector<MemberField> cache;
	if(!fields_.empty())
	{
		cache = fields_;
		fields_.clear();
	}//End if

	if(type_ == nullptr) return;

	const auto fields = type_->GetFields();

	for(int i = 0; i < type_->GetNumberOfFields(); i++)
	{
		TemplateField field = fields[i];
		fields_.emplace_back(this, *field.GetDataType(), i);
	}//End for

	for (int i = 0; i < cache.size(); i++)
	{
		if(fields_.size() > i)
		{
			if(strcmp(fields_.at(i).GetNameAndTypeLabel(), cache.at(i).GetNameAndTypeLabel()) == 0)
			{
				strcpy(fields_.at(i).GetDataBuffer(), cache.at(i).GetDataBuffer());
			}//End if
		}//End if
	}//End for
}//End RefreshFieldQuantity