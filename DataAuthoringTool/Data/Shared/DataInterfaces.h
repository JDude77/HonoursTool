#pragma once
#ifndef DATA_INTERFACES
#define DATA_INTERFACES
#include <string>
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
	virtual int Export() = 0;
};

class PrimaryData : ISave, ILoad, IExport, IDelete
{
public:
	PrimaryData(const int internalID) : internalID_(internalID){};
	virtual int Save() override = 0;
	virtual int Load() override = 0;
	virtual int Delete() override = 0;
	virtual int Export() override = 0;

	char* GetNameBuffer() { return nameBuffer_; }
	char* GetIDBuffer() { return idBuffer_; }
	int* GetNameBufferCurrentLength() { return &nameBufferCurrentLength_; }
	int* GetIDBufferCurrentLength() { return &idBufferCurrentLength; }
	static int GetBufferMax() { return bufferMax_; }
	int GetInternalID() const { return internalID_; }

protected:
	constexpr static int bufferMax_ = 32;
	int internalID_ = -1;

	char nameBuffer_[bufferMax_] = {};
	int nameBufferCurrentLength_ = 0;
	char idBuffer_[bufferMax_] = {};
	int idBufferCurrentLength = 0;
	string name_;
	string id_;
};
#endif