#include "Template.h"

#include "../../RapidJSON/document.h"
#include "../../RapidJSON/filewritestream.h"
#include "../../RapidJSON/writer.h"
using namespace rapidjson;

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
	//Create JSON document
	Document jsonDocument;
	jsonDocument.SetObject();
	auto& documentAllocator = jsonDocument.GetAllocator();

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
			case DataType::STRING:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("string"));
				break;

			case DataType::INTEGER:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("integer"));
				break;

			case DataType::FLOAT:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("float"));
				break;

			case DataType::CHAR:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("char"));
				break;

			case DataType::BOOLEAN:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("boolean"));
				break;

			case DataType::NONE: default:
				typeVal.SetString(GenericValue<UTF8<>>::StringRefType("NONE"));
		}//End switch
		jsonField.AddMember(nameVal.Move(), typeVal.Move(), documentAllocator);

		//Create rules array value
		Value rules(kArrayType);
		auto ruleParameters = field.GetValidationRuleParameters();

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
				case ALL_PRESENCE:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Presence"), true, documentAllocator);
					break;

				case STRING_MAX_LENGTH:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("String Max Length"), static_cast<int>(strtol(ruleParameters->GetBuffer(rule), nullptr, 0)), documentAllocator);
					break;

				case STRING_MIN_LENGTH:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("String Min Length"), static_cast<int>(strtol(ruleParameters->GetBuffer(rule), nullptr, 0)), documentAllocator);
					break;

				case STRING_STARTS_WITH_SUBSTRING:
					stringVal.SetString(ruleParameters->GetBuffer(rule), documentAllocator);
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("String Starts With Substring"), stringVal, documentAllocator);
					break;

				case STRING_ENDS_WITH_SUBSTRING:
					stringVal.SetString(ruleParameters->GetBuffer(rule), documentAllocator);
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("String Ends With Substring"), stringVal, documentAllocator);
					break;

				case NUMBER_IS_NOT_NEGATIVE: 
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Number Is Not Negative"), true, documentAllocator);
					break;

				case NUMBER_IS_NEGATIVE:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Number Is Negative"), true, documentAllocator);
					break;

				case NUMBER_IS_NOT_ZERO:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Number Is Not Zero"), true, documentAllocator);
					break;

				case NUMBER_IS_LESS_THAN:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Number Is Less Than"), strtof(ruleParameters->GetBuffer(rule), nullptr), documentAllocator);
					break;

				case NUMBER_IS_GREATER_THAN:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Number Is Greater Than"), strtof(ruleParameters->GetBuffer(rule), nullptr), documentAllocator);
					break;

				case INTEGER_DIVISIBLE_BY_INTEGER:
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Integer Divisible By Integer"), static_cast<int>(strtol(ruleParameters->GetBuffer(rule), nullptr, 0)), documentAllocator);
					break;

				case CHAR_IS_ONE_OF_CHARACTER_SET:
					stringVal.SetString(ruleParameters->GetBuffer(rule), documentAllocator);
					ruleVal.AddMember(GenericValue<UTF8<>>::StringRefType("Char Is One Of Character Set"), stringVal, documentAllocator);
					break;

				case NA: default: ;
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
	fields_.emplace_back(this);
	return 1;
}//End AddNewField

int Template::DeleteField(const TemplateField* field)
{
	if(const auto position = std::find(fields_.begin(), fields_.end(), *field); position != fields_.end())
	{
		fields_.erase(position);
		return 1;
	}//End if
	return 0;
}
