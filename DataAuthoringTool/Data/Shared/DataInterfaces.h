#pragma once
#ifndef DATA_INTERFACES
#define DATA_INTERFACES
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
#endif