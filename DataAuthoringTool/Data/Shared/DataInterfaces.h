#pragma once
#ifndef DATA_INTERFACES
#define DATA_INTERFACES
#include <string>
class PrimaryData;
using std::string;

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
	virtual int Validate() = 0;
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
	virtual int Export(PrimaryData* caller = nullptr) = 0;
};

class PrimaryData : ISave, ILoad, IExport, IDelete
{
public:
	PrimaryData(const int internalID) : internalID_(internalID){}
	virtual int Save() override = 0;
	virtual int Load() override = 0;
	virtual int Delete() override = 0;
	virtual int Export(PrimaryData* caller = nullptr) override = 0;

	char* GetNameBuffer() { return nameBuffer_; }
	char* GetIDBuffer() { return idBuffer_; }
	int* GetNameBufferCurrentLength() { return &nameBufferCurrentLength_; }
	int* GetIDBufferCurrentLength() { return &idBufferCurrentLength_; }
	const static int GetBufferMax() { return bufferMax_; }
	int GetInternalID() const { return internalID_; }

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
	string name_;
	string id_;
};
#endif