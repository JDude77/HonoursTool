#pragma once
#ifndef VALIDATION_RULE
#define VALIDATION_RULE

#include "DataTypes.h"

#include <vector>
using std::vector;
#include <map>
using std::map;
#include <functional>
using std::function;

struct ValidationRule
{
private:
	//Need a map to serve as a dictionary like so:
	//DataType : List of validation rules that can be used for that type
	//Unsure whether to have list of enums which can be fed to something to invoke a function or just store the functions themselves
};
#endif