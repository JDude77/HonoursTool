#include "Group.h"
#include "../Members/Member.h"
#include "../Templates/Template.h"
#include "../../RapidJSON/filewritestream.h"
#include "../../RapidJSON/prettywriter.h"
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

int Group::Validate()
{
	//TODO: Group Validate functionality extended
	for (auto& member : members_)
	{
		member.second->Validate();
	}//End for
	return 1;
}//End Validate

int Group::Delete()
{
	//TODO: Group Delete functionality
	return 1;
}//End Delete

int Group::Export(PrimaryData* caller)
{
	//TODO: Group Export functionality
	//Create the document which all members will be added to
	shared_ptr<Document> jsonDocument = std::make_shared<Document>();
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
		member->Export(this, jsonDocument);
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
		return fclose(filePath);
	}//End Exporting functionality
}//End Export

int Group::Export(PrimaryData* caller, std::shared_ptr<Document> jsonDocument)
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
		if(member.second->GetType()->GetIDBuffer() == templateToRemove->GetIDBuffer())
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