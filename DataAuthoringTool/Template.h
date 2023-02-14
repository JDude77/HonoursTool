#pragma once
#ifndef TEMPLATE
#define TEMPLATE

#include "DataInterfaces.h"

#include <string>
using std::string;
#include <map>
using std::map;
#include <vector>
using std::vector;

//Forward Declarations
class ValidationRule;

enum DataType
{
	String,
	Integer,
	Float,
	Char,
	Boolean
};

class Field : public IDelete
{
public:
	int Delete() override;

private:
	//Note: ID referred to in-program as "name", but called ID internally to indicate that the field must be uniquely named in a given template
	string id_;
	DataType dataType_;
	vector<ValidationRule> validationRules_;
};

class Template : public ISave, public ILoad, public IDelete, public IExport
{
public:
	int Save() override;
	int Load() override;
	int Delete() override;
	int Export() override;

private:
	string name_;
	string id_;
	
	vector<Field> fields_;
};
#endif