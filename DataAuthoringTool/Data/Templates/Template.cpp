#include "Template.h"

#include "../../RapidJSON/filewritestream.h"
#include "../../RapidJSON/prettywriter.h"
#include <Windows.h>

using rapidjson::Document;
using rapidjson::FileWriteStream;
using rapidjson::GenericValue;
using rapidjson::kArrayType;
using rapidjson::kObjectType;
using rapidjson::kStringType;
using rapidjson::PrettyWriter;
using rapidjson::UTF8;
using rapidjson::Value;

Template::Template(const char* ID, const int internalID) : PrimaryData(internalID)
{
	for (int i = 0 ; i < sizeof ID; i++)
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

int Template::Export(std::queue<std::string>* outputText, PrimaryData* caller)
{
	//Create JSON document
	Document jsonDocument;
	jsonDocument.SetObject();
	Document::AllocatorType& documentAllocator = jsonDocument.GetAllocator();

	//Add template name to the JSON document
	Value nameValue;
	nameValue.SetString(GetNameBuffer(), documentAllocator);
	jsonDocument.AddMember("Name", nameValue, documentAllocator);

	//Create fields array value
	Value fields(kArrayType);
	for (TemplateField field : fields_)
	{
		//Pre-copy some data to prevent premature garbage collection
		Value jsonField(kObjectType);
		Value nameVal(field.GetIDBuffer(), documentAllocator);
		Value typeVal(kStringType);

		//Get correct type string for field
		switch(*field.GetDataType())
		{
			case DataType::DATA_TYPE::STRING:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("string"));
				break;

			case DataType::DATA_TYPE::INTEGER:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("integer"));
				break;

			case DataType::DATA_TYPE::FLOAT:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("float"));
				break;

			case DataType::DATA_TYPE::CHAR:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("char"));
				break;

			case DataType::DATA_TYPE::BOOLEAN:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("boolean"));
				break;

			case DataType::DATA_TYPE::NONE: default:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("NONE"));
		}//End switch
		jsonField.AddMember(nameVal.Move(), typeVal.Move(), documentAllocator);

		//Create rules array value
		Value rules(kArrayType);
		ValidationRuleParameter* ruleParameters = field.GetValidationRuleParameters();

		//Loop through all validation rules of the field
		for (auto& [rule, active] : *field.GetValidationRules())
		{
			//If the rule isn't active, don't bother doing any of the things below, just go to the next rule
			if(active == 0) continue;

			//Preallocate some memory for recording values
			Value ruleVal(kObjectType);
			Value stringVal(kStringType);

			//Correctly grab data about the validation rules per-type
			switch (rule)
			{
				case RULES::ALL_PRESENCE:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Presence"), true, documentAllocator);
					break;

				case RULES::STRING_MAX_LENGTH:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("String Max Length"), static_cast<int>(strtol(ruleParameters->GetBuffer(rule), nullptr, 0)), documentAllocator);
					break;

				case RULES::STRING_MIN_LENGTH:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("String Min Length"), static_cast<int>(strtol(ruleParameters->GetBuffer(rule), nullptr, 0)), documentAllocator);
					break;

				case RULES::STRING_STARTS_WITH_SUBSTRING:
					stringVal.SetString(ruleParameters->GetBuffer(rule), documentAllocator);
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("String Starts With Substring"), stringVal, documentAllocator);
					break;

				case RULES::STRING_ENDS_WITH_SUBSTRING:
					stringVal.SetString(ruleParameters->GetBuffer(rule), documentAllocator);
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("String Ends With Substring"), stringVal, documentAllocator);
					break;

				case RULES::NUMBER_IS_NOT_NEGATIVE: 
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Number Is Not Negative"), true, documentAllocator);
					break;

				case RULES::NUMBER_IS_NEGATIVE:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Number Is Negative"), true, documentAllocator);
					break;

				case RULES::NUMBER_IS_NOT_ZERO:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Number Is Not Zero"), true, documentAllocator);
					break;

				case RULES::NUMBER_IS_LESS_THAN:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Number Is Less Than"), strtof(ruleParameters->GetBuffer(rule), nullptr), documentAllocator);
					break;

				case RULES::NUMBER_IS_GREATER_THAN:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Number Is Greater Than"), strtof(ruleParameters->GetBuffer(rule), nullptr), documentAllocator);
					break;

				case RULES::INTEGER_DIVISIBLE_BY_INTEGER:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Integer Divisible By Integer"), static_cast<int>(strtol(ruleParameters->GetBuffer(rule), nullptr, 0)), documentAllocator);
					break;

				case RULES::CHAR_IS_ONE_OF_CHARACTER_SET:
					stringVal.SetString(ruleParameters->GetBuffer(rule), documentAllocator);
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Char Is One Of Character Set"), stringVal, documentAllocator);
					break;

				case RULES::NA: default: ;
			}//End switch

			//Add the constructed rule value to the rules array
			rules.PushBack(ruleVal, documentAllocator);
		}//End rules for

		//Only add the rules array to the field value if there are actually active rules
		if(!rules.Empty()) jsonField.AddMember(GenericValue<UTF8<>>::StringRefType("Active Validation Rules"), rules.Move(), documentAllocator);

		//Add the completed field to the field array
		fields.PushBack(jsonField.Move(), documentAllocator);
	}//End fields for

	//Add the completed collection of fields to the JSON file
	jsonDocument.AddMember(GenericValue<UTF8<>>::StringRefType(GetIDBuffer()), fields, documentAllocator);
	{
		//Construct file name
		string fileName;
		fileName.append(idBuffer_);
		fileName.append("_TEMPLATE.json");

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
		string text = "Template ";
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
}//End Export (Template)

//Export call for adding a template to a group
int Template::Export(std::queue<std::string>* outputText, PrimaryData* caller, std::shared_ptr<Document> jsonDocument)
{
	//Not needed due to the member implicitly handling this when it is exported - the templates don't have full detail exported in groups
	return 1;
}//End Export

int Template::Delete()
{
	memset(idBuffer_, 0, sizeof idBuffer_);
	idBufferCurrentLength_ = 0;
	memset(nameBuffer_, 0, sizeof nameBuffer_);
	nameBufferCurrentLength_ = 0;
	fields_.clear();
	return 1;
}//End Delete (Template)

TemplateField* Template::GetFieldAtIndex(const int index)
{
	return &fields_[index];
}//End GetFieldAtIndex

int Template::AddNewField()
{
	fields_.emplace_back(this);
	return 1;
}//End AddNewField

int Template::DeleteField(const TemplateField* field)
{
	if(const auto position = std::ranges::find(fields_, *field); position != fields_.end())
	{
		fields_.erase(position);
		return 1;
	}//End if
	return 0;
}//End DeleteField
