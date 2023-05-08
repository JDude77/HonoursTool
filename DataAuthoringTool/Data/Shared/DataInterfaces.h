#pragma once
#ifndef DATA_INTERFACES
#define DATA_INTERFACES
#include "../../RapidJSON/document.h"

#include <memory>
#include <string>
#include <queue>

using std::string;
using std::queue;
using std::shared_ptr;
using rapidjson::Document;

class PrimaryData;

class ISave
{
	public:
	virtual int Save() = 0;
};

class ILoad
{
	public:
	virtual int Load() = 0;
};

class IValidate
{
	public:
	virtual int Validate(queue<string>* outputText) = 0;
};

class IDelete
{
	public:
	virtual int Delete() = 0;
};

class IExport
{
	public:
	//Caller pointer determines whether the JSON should be part of a larger JSON file (has caller) or an individual one (null pointer)
	virtual int Export(queue<string>* outputText, PrimaryData* caller = nullptr) = 0;
	virtual int Export(queue<string>* outputText, PrimaryData* caller, shared_ptr<Document> jsonDocument) = 0;
};

class PrimaryData : ISave, ILoad, IExport, IDelete
{
public:
	PrimaryData(const int internalID) : internalID_(internalID){}
	int Save() override = 0;
	int Load() override = 0;
	int Delete() override = 0;
	int Export(queue<string>* outputText, PrimaryData* caller = nullptr) override = 0;
	int Export(queue<string>* outputText, PrimaryData* caller, shared_ptr<Document> jsonDocument) override = 0;

	char* GetNameBuffer() { return nameBuffer_; }
	char* GetIDBuffer() { return idBuffer_; }

	int* GetNameBufferCurrentLength() { return &nameBufferCurrentLength_; }
	int* GetIDBufferCurrentLength() { return &idBufferCurrentLength_; }

	static int GetBufferMax() { return bufferMax_; }
	[[nodiscard]] int GetInternalID() const { return internalID_; }
	[[nodiscard]] virtual bool IsEmpty() const { return strlen(idBuffer_) == 0 && strlen(nameBuffer_) == 0; }

	void UpdateNameBuffer()
	{
		for (int i = nameBufferCurrentLength_ ; i < bufferMax_; i++)
		{
			nameBuffer_[i] = '\0';
		}//End for
	}//End UpdateNameBuffer

	void UpdateIDBuffer()
	{
		for (int i = idBufferCurrentLength_ ; i < bufferMax_; i++)
		{
			idBuffer_[i] = '\0';
		}//End for
	}//End UpdateIDBuffer

protected:
	constexpr static int bufferMax_ = 32;
	int internalID_ = -1;

	char nameBuffer_[bufferMax_] = {};
	int nameBufferCurrentLength_ = 0;
	char idBuffer_[bufferMax_] = {};
	int idBufferCurrentLength_ = 0;
};
#endif
