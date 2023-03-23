#pragma once
#ifndef VALIDATION_RULE
#define VALIDATION_RULE

#include <algorithm>

#include "../Shared/DataTypes.h"

#include <vector>
using std::vector;
#include <map>
using std::map;
#include <functional>
using std::function;
#include <any>

//Enum representing each individual possible rule
enum RULES
{
	NA,
	ALL_PRESENCE,
	STRING_MAX_LENGTH,
	STRING_MIN_LENGTH,
	STRING_STARTS_WITH_SUBSTRING,
	STRING_ENDS_WITH_SUBSTRING,
	NUMBER_IS_NOT_NEGATIVE,
	NUMBER_IS_NEGATIVE,
	NUMBER_IS_NOT_ZERO,
	NUMBER_IS_LESS_THAN,
	NUMBER_IS_GREATER_THAN,
	INTEGER_DIVISIBLE_BY_INTEGER,
	CHAR_IS_ONE_OF_CHARACTER_SET
};

//Struct which wraps the validation functions themselves for look-up and call in map
//It feels cursed. Very cursed.
struct ValidationFunction
{
	template<typename F>
	ValidationFunction(F&& fun) : ValidationFunction(function(std::forward<F>(fun))) {}

	template<typename ... Args>
	ValidationFunction(function<bool(Args...)> fun) : any_(fun) {}

	template<typename ... Args>
	bool operator()(Args&& ... args)
	{
		return std::invoke(std::any_cast<function<bool(Args...)>>(any_), std::forward<Args>(args)...);
	}//End () operator

	static bool Presence(const string& dataBuffer)
	{
		return !dataBuffer.empty();
	}

	static bool StringMaxLength(const string& dataBuffer, const int& maxLength)
	{
		return dataBuffer.size() <= maxLength;
	}

	static bool StringMinLength(const string& dataBuffer, const int& minLength)
	{
		return dataBuffer.size() >= minLength;
	}

	static bool StringStartsWithSubstring(const string& dataBuffer, const string& subString)
	{
		const auto temp = dataBuffer.substr(0, subString.size());
		const auto result = subString.compare(temp);
		return result == 0 ? true : false;
	}

	static bool StringEndsWithSubstring(const string& dataBuffer, const string& subString)
	{
		const auto temp = dataBuffer.substr(dataBuffer.size() - subString.size(), subString.size());
		const auto result = subString.compare(temp);
		return result == 0 ? true : false;
	}

	static bool NumberIsNotNegative(const string& dataBuffer)
	{
		return dataBuffer.at(0) != '-';
	}

	static bool NumberIsNegative(const string& dataBuffer)
	{
		return dataBuffer.at(0) == '-';
	}

	static bool NumberIsNotZero(const string& dataBuffer)
	{
		const auto number = strtol(dataBuffer.c_str(), nullptr, 0);
		return number != 0;
	}

	static bool NumberIsGreaterThan(const string& dataBuffer, const float& greaterThan)
	{
		const auto number = strtof(dataBuffer.c_str(), nullptr);
		return number > greaterThan;
	}

	static bool NumberIsLessThan(const string& dataBuffer, const float& lessThan)
	{
		const auto number = strtof(dataBuffer.c_str(), nullptr);
		return number < lessThan;
	}

	static bool IntegerDivisibleByInteger(const string& dataBuffer, const int& divisor)
	{
		const auto number = strtol(dataBuffer.c_str(), nullptr, 0);
		return number % divisor == 0;
	}

	static bool CharIsOneOfCharacterSet(const string& dataBuffer, const vector<char>& characterSet)
	{
		auto checkChar = [](char character)
		{
			return [character](const char otherCharacter) { return character == otherCharacter; };
		};

		if(const char dataCharacter = dataBuffer.at(0); std::ranges::any_of(characterSet, checkChar(dataCharacter)))
		{
			return true;
		}//End if
		return false;
	}

private:
	std::any any_;
};

class ValidationRule
{
	inline static const map<const DataType::DATA_TYPE, const vector<RULES>> rulesPerType_
	{
		//None (empty)
		{
			DataType::NONE,
			vector{ NA }//End None Rules
		},//End None Entry

		//Boolean (empty)
		{
			DataType::BOOLEAN,
			vector{ NA }//End Boolean Rules
		},//End Boolean Entry

		//String
		{
			DataType::STRING,
			vector
			{
				ALL_PRESENCE,
				STRING_MAX_LENGTH,
				STRING_MIN_LENGTH,
				STRING_STARTS_WITH_SUBSTRING,
				STRING_ENDS_WITH_SUBSTRING
			}//End String Rules
		},//End String Entry

		//Integer
		{
			DataType::INTEGER,
			vector
			{
				ALL_PRESENCE,
				NUMBER_IS_NOT_NEGATIVE,
				NUMBER_IS_NEGATIVE,
				NUMBER_IS_NOT_ZERO,
				NUMBER_IS_LESS_THAN,
				NUMBER_IS_GREATER_THAN,
				INTEGER_DIVISIBLE_BY_INTEGER
			}//End Integer Rules
		},//End Integer Entry

		//Float
		{
			DataType::FLOAT,
			vector
			{
				ALL_PRESENCE,
				NUMBER_IS_NOT_NEGATIVE,
				NUMBER_IS_NEGATIVE,
				NUMBER_IS_NOT_ZERO,
				NUMBER_IS_LESS_THAN,
				NUMBER_IS_GREATER_THAN
			}//End Float Rules
		},//End Float Entry

		//Char
		{
			DataType::CHAR,
			vector
			{
				ALL_PRESENCE,
				CHAR_IS_ONE_OF_CHARACTER_SET
			}//End Char Rules
		},//End Char Entry
	};//End Map Initialization

	inline static const map<const RULES, ValidationFunction> validationRuleFunctions_
	{
		{ALL_PRESENCE,					ValidationFunction(function(&ValidationFunction::Presence))},
		{STRING_MAX_LENGTH,				ValidationFunction(function(&ValidationFunction::StringMaxLength))},
		{STRING_STARTS_WITH_SUBSTRING,	ValidationFunction(function(&ValidationFunction::StringStartsWithSubstring))},
		{STRING_MIN_LENGTH,				ValidationFunction(function(&ValidationFunction::StringMinLength))},
		{STRING_ENDS_WITH_SUBSTRING,		ValidationFunction(function(&ValidationFunction::StringEndsWithSubstring))},
		{NUMBER_IS_NOT_NEGATIVE,			ValidationFunction(function(&ValidationFunction::NumberIsNotNegative))},
		{NUMBER_IS_NEGATIVE,				ValidationFunction(function(&ValidationFunction::NumberIsNegative))},
		{NUMBER_IS_NOT_ZERO,				ValidationFunction(function(&ValidationFunction::NumberIsNotZero))},
		{NUMBER_IS_GREATER_THAN,			ValidationFunction(function(&ValidationFunction::NumberIsGreaterThan))},
		{NUMBER_IS_LESS_THAN,				ValidationFunction(function(&ValidationFunction::NumberIsLessThan))},
		{INTEGER_DIVISIBLE_BY_INTEGER,	ValidationFunction(function(&ValidationFunction::IntegerDivisibleByInteger))},
		{CHAR_IS_ONE_OF_CHARACTER_SET,	ValidationFunction(function(&ValidationFunction::CharIsOneOfCharacterSet))},
	};

	inline static const map<const RULES, const string> validationRuleLabels_
	{
		{ALL_PRESENCE,					"Must not be left blank/empty"},
		{STRING_MAX_LENGTH,				"Text must not be longer than maximum length"},
		{STRING_MIN_LENGTH,				"Text must not be shorter than minimum length"},
		{STRING_STARTS_WITH_SUBSTRING,	"Text must start with specific text"},
		{STRING_ENDS_WITH_SUBSTRING,		"Text must end with specific text"},
		{NUMBER_IS_NOT_NEGATIVE,			"Number must not be negative"},
		{NUMBER_IS_NOT_ZERO,				"Number must not be zero"},
		{NUMBER_IS_NEGATIVE,				"Number must be negative"},
		{NUMBER_IS_GREATER_THAN,			"Number must be greater than specific number"},
		{NUMBER_IS_LESS_THAN,				"Number must be less than specific number"},
		{INTEGER_DIVISIBLE_BY_INTEGER,	"Whole number must be divisible by other whole number"},
		{CHAR_IS_ONE_OF_CHARACTER_SET,	"Character must be one of a set of characters"},
	};


public:
	static const string& GetValidationRuleLabel(const RULES& rule)
	{
		return validationRuleLabels_.at(rule);
	}//End GetValidationRuleLabelsOfType

	static const vector<string> GetValidationRuleLabels(const DataType::DATA_TYPE type)
	{
		switch(type)
		{
			case DataType::NONE: case DataType::BOOLEAN: return {};

			case DataType::STRING:
				return vector
				{
					validationRuleLabels_.at(ALL_PRESENCE),
					validationRuleLabels_.at(STRING_MAX_LENGTH),
					validationRuleLabels_.at(STRING_MIN_LENGTH),
					validationRuleLabels_.at(STRING_STARTS_WITH_SUBSTRING),
					validationRuleLabels_.at(STRING_ENDS_WITH_SUBSTRING)
				};
			case DataType::INTEGER:
				return vector
				{
					validationRuleLabels_.at(ALL_PRESENCE),
					validationRuleLabels_.at(NUMBER_IS_NOT_NEGATIVE),
					validationRuleLabels_.at(NUMBER_IS_NEGATIVE),
					validationRuleLabels_.at(NUMBER_IS_NOT_ZERO),
					validationRuleLabels_.at(NUMBER_IS_LESS_THAN),
					validationRuleLabels_.at(NUMBER_IS_GREATER_THAN),
					validationRuleLabels_.at(INTEGER_DIVISIBLE_BY_INTEGER)
				};
			case DataType::FLOAT: 
				return vector
				{
					validationRuleLabels_.at(ALL_PRESENCE),
					validationRuleLabels_.at(NUMBER_IS_NOT_NEGATIVE),
					validationRuleLabels_.at(NUMBER_IS_NEGATIVE),
					validationRuleLabels_.at(NUMBER_IS_NOT_ZERO),
					validationRuleLabels_.at(NUMBER_IS_LESS_THAN),
					validationRuleLabels_.at(NUMBER_IS_GREATER_THAN)
				};
			case DataType::CHAR:
				return vector
				{
					validationRuleLabels_.at(ALL_PRESENCE),
					validationRuleLabels_.at(CHAR_IS_ONE_OF_CHARACTER_SET)
				};
		}//End switch
		return {};
	} //End GetValidationRuleLabels

	static const vector<RULES>& GetValidationRuleLabelsOfType(const int type)
	{
		return rulesPerType_.at(static_cast<DataType::DATA_TYPE>(type));
	}//End GetValidationRuleLabelsOfType

	static const vector<RULES>& GetValidationRuleLabelsOfType(const DataType::DATA_TYPE type)
	{
		return rulesPerType_.at(type);
	}//End GetValidationRuleLabelsOfType
};


#endif