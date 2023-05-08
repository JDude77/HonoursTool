#include "Group.h"

#include <Windows.h>
#include "../Members/Member.h"
#include "../Templates/Template.h"
#include "../../RapidJSON/filewritestream.h"
#include "../../RapidJSON/prettywriter.h"
#include <ranges>
using namespace rapidjson;

int Group::Save()
{
	//TODO: Group Save functionality
	return 1;
}//End Save

int Group::Load()
{
	//TODO: Group Load functionality
	return 1;
}//End Load

int Group::Validate(std::queue<std::string>* outputText)
{
	//TODO: Group Validate functionality extended
	string text;
	text.append("Group ");
	text.append(idBuffer_);
	text.append(" Validating All Members...");
	outputText->push(text);
	for (const auto& member : members_ | std::views::values)
	{
		member->Validate(outputText);
	}//End for
	text.clear();
	text.append("Group ");
	text.append(idBuffer_);
	text.append(" Validation Complete");
	outputText->push(text);
	return 1;
}//End Validate

int Group::Delete()
{
	memset(idBuffer_, 0, sizeof idBuffer_);
	idBufferCurrentLength_ = 0;
	memset(nameBuffer_, 0, sizeof nameBuffer_);
	nameBufferCurrentLength_ = 0;
	members_.clear();
	templates_.clear();
	return 1;
}//End Delete

int Group::Export(std::queue<std::string>* outputText, PrimaryData* caller)
{
	//Create the document which all members will be added to
	const shared_ptr<Document> jsonDocument = std::make_shared<Document>();
	jsonDocument->SetObject();
	Document::AllocatorType& jsonDocAlloc = jsonDocument->GetAllocator();

	//Get group name and put it in the doc
	Value nameValue;
	nameValue.SetString(GenericValue<UTF8<>>::StringRefType(GetNameBuffer()));
	jsonDocument->AddMember("Name", nameValue, jsonDocAlloc);

	//Loop through every member
	for(const auto& member : members_ | std::views::values)
	{
		//Add the member to the document object
		member->Export(outputText, this, jsonDocument);
	}//End for

	//By here we're ready to just export a JSON document as per usual
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
		jsonDocument->Accept(writer);

		//Close file
		const int success = fclose(filePath);

		//Output text
		string text = "Group ";
		text.append(idBuffer_);
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

		return success;
	}//End Exporting functionality
}//End Export

int Group::Export(std::queue<std::string>* outputText, PrimaryData* caller, std::shared_ptr<Document> jsonDocument)
{
	//Nothing to do here for now - this version is mainly just used for calling on members/templates in groups
	return -1;
}//End Export

vector<shared_ptr<Member>> Group::GetMembers()
{
	vector<shared_ptr<Member>> members;
	members.reserve(members_.size());
	for (auto& member : members_ | std::views::values)
	{
		members.push_back(member);
	}//End for
	return members;
}//End GetMembers

vector<shared_ptr<Template>> Group::GetTemplates()
{
	vector<shared_ptr<Template>> templates;
	templates.reserve(templates_.size());
	for (auto& temp : templates_ | std::views::values)
	{
		templates.push_back(temp);
	}//End for
	return templates;
}///End GetTemplates

int Group::AddTemplateToGroup(const shared_ptr<Template>& newTemplate)
{
	auto id = newTemplate->GetIDBuffer();
	const auto ptr = newTemplate.get();
	//Check to ensure the template isn't already in the group
	for (auto& [existingID, pointer] : templates_)
	{
		if(existingID == id || ptr == pointer.get())
		{
			return 0;
		}//End if
	}//End for

	//Reset flags and cached internal IDs
	addNewTemplateFlag_ = false;
	cachedIDOfTemplateToAdd_ = -1;

	//Add the template to the group
	templates_.emplace_back(id, newTemplate);
	return 1;
}//End AddTemplateToGroup


int Group::AddMemberToGroup(const shared_ptr<Member>& newMember)
{
	auto id = newMember->GetIDBuffer();
	const auto ptr = newMember.get();
	//Check to ensure the member isn't already in the group
	for (auto& [existingID, pointer] : members_)
	{
		if(existingID == id || ptr == pointer.get())
		{
			return 0;
		}//End if
	}//End for

	//Add the member to the group
	members_.emplace_back(id, newMember);
	return 1;
}//End AddMemberToGroup

int Group::RemoveTemplateFromGroup(const shared_ptr<Template>& templateToRemove)
{
	std::pair<string, shared_ptr<Template>> finderPair;
	finderPair.first = templateToRemove->GetIDBuffer();
	finderPair.second = templateToRemove;

	const auto it = std::ranges::find(templates_, finderPair);

	//Check to see if it's actually in the group, return 0 if it isn't
	if(it == templates_.end()) return 0;

	//Remove all members of that template from the group
	vector<std::ranges::borrowed_iterator_t<vector<pair<string, shared_ptr<Member>>>&>> membersToErase;
	for (auto& member : members_)
	{
		if(member.second->GetType()->GetInternalID() == templateToRemove->GetInternalID())
		{
			//Safely mark all of them for deletion by grabbing a reference to their iterator positions
			membersToErase.push_back(std::ranges::find(members_, member));
		}//End if
	}//End for

	//Go through the iterators and remove the members from the original list
	for (const auto& memberToErase : membersToErase)
	{
		members_.erase(memberToErase);
	}//End for

	//Remove the template from the group
	templates_.erase(it);

	//Try and fix the tabs issue
	if(templates_.empty()) activeTemplateTab_ = -1;

	return 1;
}//End RemoveTemplateFromGroup

int Group::RemoveMemberFromGroup(const shared_ptr<Member>& memberToRemove)
{
	std::pair<string, shared_ptr<Member>> finderPair;
	finderPair.first = memberToRemove->GetIDBuffer();
	finderPair.second = memberToRemove;

	const auto it = std::ranges::find(members_, finderPair);

	//Check to see if it's actually in the group, return 0 if it isn't
	if(it == members_.end()) return 0;

	//If it IS in the group, erase/remove it from the group
	members_.erase(it);

	return 1;
}//End RemoveMemberFromGroup

void Group::FlagTemplateToAdd(const int templateID)
{
	cachedIDOfTemplateToAdd_ = templateID;
	addNewTemplateFlag_ = true;
}//End FlagTemplateToAdd