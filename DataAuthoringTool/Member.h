#pragma once
#ifndef MEMBER
#define MEMBER

#include "DataInterfaces.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

//Forward Declarations
class Template;
class Field;

class FieldData : public IDelete, public IValidate
{
public:
	int Validate() override;
	int Delete() override;

private:
	Field* field_;
};

class Member : public ISave, public ILoad, public IExport, public IDelete, public IValidate
{
public:
	int Save() override;
	int Load() override;
	int Export() override;
	int Delete() override;
	int Validate() override;

private:
	string id_;
	string name_;
	Template* type_ = nullptr;
};
#endif