#include "Member.h"

#include "../Templates/Template.h"
#include "../../RapidJSON/filewritestream.h"
#include "../../RapidJSON/prettywriter.h"
#include <Windows.h>

using rapidjson::Document;
using rapidjson::FileWriteStream;
using rapidjson::GenericValue;
using rapidjson::kArrayType;
using rapidjson::kObjectType;
using rapidjson::PrettyWriter;
using rapidjson::UTF8;
using rapidjson::Value;

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

int Member::Export(std::queue<std::string>* outputText, PrimaryData* caller)
{
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
			case DataType::DATA_TYPE::STRING:
				{
					Value stringVal;
					stringVal.SetString(GenericValue<UTF8<>>::StringRefType(field.GetDataBuffer()));
					jsonDocument.AddMember(name, stringVal, allocator);
				}//End String data handling
				break;

			case DataType::DATA_TYPE::INTEGER:
				{
					Value integerVal;
					int data = strtol(field.GetDataBuffer(), nullptr, 0);
					integerVal.SetInt(data);
					jsonDocument.AddMember(name, integerVal, allocator);
				}//End Integer data handling
				break;

			case DataType::DATA_TYPE::FLOAT:
				{
					Value floatVal;
					float data = strtof(field.GetDataBuffer(), nullptr);
					floatVal.SetFloat(data);
					jsonDocument.AddMember(name, floatVal, allocator);
				}//End Float data handling
				break;

			case DataType::DATA_TYPE::CHAR:
				{
					Value charVal;
					charVal.SetString(GenericValue<UTF8<>>::StringRefType(field.GetDataBuffer()));
					jsonDocument.AddMember(name, charVal, allocator);
				}//End Char data handling
				break;

			case DataType::DATA_TYPE::BOOLEAN:
				{
					Value booleanVal;
					booleanVal.SetBool(field.GetBooleanData());
					jsonDocument.AddMember(name, booleanVal, allocator);
				}//End Boolean data handling
				break;

			case DataType::DATA_TYPE::NONE: default: break;
		}//End switch
	}//End for
	{
		//Construct file name
		string fileName;
		fileName.append(idBuffer_);
		fileName.append("_MEMBER.json");

		//Set up buffer/stream/file
		FILE* filePath = fopen(fileName.c_str(), "wb");
		char writeBuffer[65536];
		FileWriteStream stream(filePath, writeBuffer, sizeof writeBuffer);

		//Write to file
		PrettyWriter writer(stream);
		jsonDocument.Accept(writer);

		//Close file
		int success = fclose(filePath);

		//Output text
		string text = "Member ";
		text.append(idBuffer_);

		if(success == 0)
		{
			text.append(" exported successfully!");
			outputText->push(text);

			//Get file path exported to
			text = "File located at: \"";
			char fullFilePath[MAX_PATH];
			GetModuleFileNameA(nullptr, fullFilePath, MAX_PATH);
			const std::string::size_type pos = std::string(fullFilePath).find_last_of("\\/");
			text.append(std::string(fullFilePath).substr(0, pos));
			text.append("\" in ");
			text.append(fileName);
			outputText->push(text);

			const std::wstring fileNameToWString = std::wstring(fileName.begin(), fileName.end());
			const LPCWSTR fileAsCString = fileNameToWString.c_str();

			//Open the file in an editor
			ShellExecute(nullptr, nullptr, fileAsCString, nullptr, nullptr, SW_SHOW);
		}//End if
		else
		{
			text.append(" did not export successfully.");
			outputText->push(text);

			text = "File close function returned with value: ";
			text.append(std::to_string(success));
			outputText->push(text);

			text = "Please send a report about this error to the developer.";
			outputText->push(text);
		}//End else

		return success;
	}//End Exporting functionality
}//End Export

//Export call for adding a member to a group JSON file
int Member::Export(std::queue<std::string>* outputText, PrimaryData* caller, std::shared_ptr<Document> jsonDocument)
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

	//Loop through every member field
	for (const MemberField& field : fields_)
	{
		auto name = GenericValue<UTF8<>>::StringRefType(field.GetName());

		Value fieldValue(kObjectType);

		//Correctly grab the data in a JSON value for each type
		switch(field.GetDataType())
		{
			case DataType::DATA_TYPE::STRING:
			{
				Value stringVal;
				stringVal.SetString(GenericValue<UTF8<>>::StringRefType(field.GetDataBuffer()));
				fieldValue.AddMember(name, stringVal, allocator);
			}//End String data handling
			break;

			case DataType::DATA_TYPE::INTEGER:
			{
				Value integerVal;
				int data = strtol(field.GetDataBuffer(), nullptr, 0);
				integerVal.SetInt(data);
				fieldValue.AddMember(name, integerVal, allocator);
			}//End Integer data handling
			break;

			case DataType::DATA_TYPE::FLOAT:
			{
				Value floatVal;
				float data = strtof(field.GetDataBuffer(), nullptr);
				floatVal.SetFloat(data);
				fieldValue.AddMember(name, floatVal, allocator);
			}//End Float data handling
			break;

			case DataType::DATA_TYPE::CHAR:
			{
				Value charVal;
				charVal.SetString(GenericValue<UTF8<>>::StringRefType(field.GetDataBuffer()));
				fieldValue.AddMember(name, charVal, allocator);
			}//End Char data handling
			break;

			case DataType::DATA_TYPE::BOOLEAN:
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
	memset(idBuffer_, 0, sizeof idBuffer_);
	idBufferCurrentLength_ = 0;
	memset(nameBuffer_, 0, sizeof nameBuffer_);
	nameBufferCurrentLength_ = 0;
	fields_.clear();
	return 1;
}//End Delete

int Member::Validate(std::queue<std::string>* outputText)
{
	//Ouput text to member window informing what's going on
	string text;
	text.append("Member ");
	text.append(idBuffer_);
	text.append(" Validating Data...");
	outputText->push(text);

	//Validate every field
	for (MemberField& field : fields_)
	{
		field.Validate(outputText);
	}//End for

	//Output text to inform validation completing
	text.clear();
	text.append("Member ");
	text.append(idBuffer_);
	text.append(" Validation Complete");
	outputText->push(text);

	return 1;
}//End Validate

bool Member::IsEmpty() const
{
	return PrimaryData::IsEmpty() && fields_.empty() && (type_ == nullptr || type_->GetInternalID() == -1);
}//End IsEmpty

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

	//Clear the fields when switching data type
	if(!fields_.empty()) fields_.clear();

	const vector<TemplateField> fields = type_->GetFields();

	//Add all the new fields to the member
	fields_.reserve(fields.size());
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
	//Cache the fields in the member then clear the actual values
	vector<MemberField> cache;
	if(!fields_.empty())
	{
		cache = fields_;
		fields_.clear();
	}//End if

	//If type was set to nothing, then there's nothing else to do
	if(type_ == nullptr) return;

	const vector<TemplateField> fields = type_->GetFields();

	//Add the fields back into the member
	for(int i = 0; i < type_->GetNumberOfFields(); i++)
	{
		TemplateField field = fields[i];
		fields_.emplace_back(this, *field.GetDataType(), i);
	}//End for

	//Loop through the cached fields
	for (MemberField& cacheField : cache)
	{
		//Loop through the new fields
		for (MemberField& field : fields_)
		{
			//If the existing field name and type match the new field and type
			if(strcmp(field.GetNameAndTypeLabel(), cacheField.GetNameAndTypeLabel()) == 0)
			{
				//Add the data back into the data buffer from the cache
				strcpy(field.GetDataBuffer(), cacheField.GetDataBuffer());
				field.SetDataBufferCurrentSize(*cacheField.GetDataBufferCurrentSize());
				break;
			}//End if
		}//End for
	}//End for
}//End RefreshFieldQuantity