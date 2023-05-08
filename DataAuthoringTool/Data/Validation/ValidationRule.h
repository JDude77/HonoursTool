#pragma once
#ifndef VALIDATION_RULE
#define VALIDATION_RULE
#include "../Shared/DataTypes.h"

#include <vector>
#include <map>
#include <functional>
#include <any>

using std::vector;
using std::map;
using std::function;

//Enum representing each individual possible rule
enum class RULES
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
	template<typename ... Args>
	ValidationFunction(function<bool(Args...)> fun) : any_(fun) {}

	static bool Presence(const char* dataBuffer)
	{
		return strlen(dataBuffer) != 0;
	}//End Presence

	static bool StringMaxLength(const char* dataBuffer, const char* maxLength)
	{
		return strlen(dataBuffer) <= strtol(maxLength, nullptr, 0);
	}//End StringMaxLength

	static bool StringMinLength(const char* dataBuffer, const char* minLength)
	{
		return strlen(dataBuffer) >= strtol(minLength, nullptr, 0);
	}//End StringMinLength

	static bool StringStartsWithSubstring(const char* dataBuffer, const char* subString)
	{
		if(strlen(dataBuffer) == 0) return false;
		const string data(dataBuffer);
		const string sub(subString);
		const string temp = data.substr(0, sub.size());
		const int result = sub.compare(temp);
		return result == 0 ? true : false;
	}//End StringStartsWithSubstring

	static bool StringEndsWithSubstring(const char* dataBuffer, const char* subString)
	{
		if(strlen(dataBuffer) == 0) return false;
		const string data(dataBuffer);
		const string sub(subString);
		const string temp = data.substr(data.size() - sub.size(), sub.size());
		const int result = sub.compare(temp);
		return result == 0 ? true : false;
	}//End StringEndsWithSubstring

	static bool NumberIsNotNegative(const char* dataBuffer)
	{
		return dataBuffer[0] != '-';
	}//End NumberIsNotNegative

	static bool NumberIsNegative(const char* dataBuffer)
	{
		return dataBuffer[0] == '-';
	}//End NumberIsNegative

	static bool NumberIsNotZero(const char* dataBuffer)
	{
		const long number = strtol(dataBuffer, nullptr, 0);
		return number != 0;
	}//End NumberIsNotZero

	static bool NumberIsGreaterThan(const char* dataBuffer, const char* greaterThan)
	{
		const float number = strtof(dataBuffer, nullptr);
		return number > strtof(greaterThan, nullptr);
	}//End NumberIsGreaterThan

	static bool NumberIsLessThan(const char* dataBuffer, const char* lessThan)
	{
		const float number = strtof(dataBuffer, nullptr);
		return number < strtof(lessThan, nullptr);
	}//End NumberIsLessThan

	static bool IntegerDivisibleByInteger(const char* dataBuffer, const char* divisor)
	{
		const long number = strtol(dataBuffer, nullptr, 0);
		if(number == 0) return false;
		return number % strtol(divisor, nullptr, 0) == 0;
	}//End IntegerDivisibleByInteger

	static bool CharIsOneOfCharacterSet(const char* dataBuffer, const char* characterSet)
	{
		for (int i = 0; i < strlen(characterSet); i++)
		{
			if(dataBuffer[0] == characterSet[i]) return true;
		}//End for
		return false;
	}//End CharIsOneOfCharSet

private:
	std::any any_;
};

class ValidationRule
{
	inline static const map<const DataType::DATA_TYPE, const vector<RULES>> rulesPerType_
	{
		//None (empty)
		{
			DataType::DATA_TYPE::NONE,
			vector{RULES::NA }//End None Rules
		},//End None Entry

		//Boolean (empty)
		{
			DataType::DATA_TYPE::BOOLEAN,
			vector{RULES::NA }//End Boolean Rules
		},//End Boolean Entry

		//String
		{
			DataType::DATA_TYPE::STRING,
			vector
			{
				RULES::ALL_PRESENCE,
				RULES::STRING_MAX_LENGTH,
				RULES::STRING_MIN_LENGTH,
				RULES::STRING_STARTS_WITH_SUBSTRING,
				RULES::STRING_ENDS_WITH_SUBSTRING
			}//End String Rules
		},//End String Entry

		//Integer
		{
			DataType::DATA_TYPE::INTEGER,
			vector
			{
				RULES::ALL_PRESENCE,
				RULES::NUMBER_IS_NOT_NEGATIVE,
				RULES::NUMBER_IS_NEGATIVE,
				RULES::NUMBER_IS_NOT_ZERO,
				RULES::NUMBER_IS_LESS_THAN,
				RULES::NUMBER_IS_GREATER_THAN,
				RULES::INTEGER_DIVISIBLE_BY_INTEGER
			}//End Integer Rules
		},//End Integer Entry

		//Float
		{
			DataType::DATA_TYPE::FLOAT,
			vector
			{
				RULES::ALL_PRESENCE,
				RULES::NUMBER_IS_NOT_NEGATIVE,
				RULES::NUMBER_IS_NEGATIVE,
				RULES::NUMBER_IS_NOT_ZERO,
				RULES::NUMBER_IS_LESS_THAN,
				RULES::NUMBER_IS_GREATER_THAN
			}//End Float Rules
		},//End Float Entry

		//Char
		{
			DataType::DATA_TYPE::CHAR,
			vector
			{
				RULES::ALL_PRESENCE,
				RULES::CHAR_IS_ONE_OF_CHARACTER_SET
			}//End Char Rules
		},//End Char Entry
	};//End Map Initialization
public:
	inline static const map<const RULES, ValidationFunction> validationRuleFunctions_
	{
		{RULES::ALL_PRESENCE,						ValidationFunction(function(&ValidationFunction::Presence))},
		{RULES::STRING_MAX_LENGTH,				ValidationFunction(function(&ValidationFunction::StringMaxLength))},
		{RULES::STRING_STARTS_WITH_SUBSTRING,		ValidationFunction(function(&ValidationFunction::StringStartsWithSubstring))},
		{RULES::STRING_MIN_LENGTH,				ValidationFunction(function(&ValidationFunction::StringMinLength))},
		{RULES::STRING_ENDS_WITH_SUBSTRING,		ValidationFunction(function(&ValidationFunction::StringEndsWithSubstring))},
		{RULES::NUMBER_IS_NOT_NEGATIVE,			ValidationFunction(function(&ValidationFunction::NumberIsNotNegative))},
		{RULES::NUMBER_IS_NEGATIVE,				ValidationFunction(function(&ValidationFunction::NumberIsNegative))},
		{RULES::NUMBER_IS_NOT_ZERO,				ValidationFunction(function(&ValidationFunction::NumberIsNotZero))},
		{RULES::NUMBER_IS_GREATER_THAN,			ValidationFunction(function(&ValidationFunction::NumberIsGreaterThan))},
		{RULES::NUMBER_IS_LESS_THAN,				ValidationFunction(function(&ValidationFunction::NumberIsLessThan))},
		{RULES::INTEGER_DIVISIBLE_BY_INTEGER,		ValidationFunction(function(&ValidationFunction::IntegerDivisibleByInteger))},
		{RULES::CHAR_IS_ONE_OF_CHARACTER_SET,		ValidationFunction(function(&ValidationFunction::CharIsOneOfCharacterSet))},
	};
private:
	inline static const map<const RULES, const string> validationRuleLabels_
	{
		{RULES::ALL_PRESENCE,						"Must not be left blank/empty"},
		{RULES::STRING_MAX_LENGTH,				"Text must not be longer than maximum length"},
		{RULES::STRING_MIN_LENGTH,				"Text must not be shorter than minimum length"},
		{RULES::STRING_STARTS_WITH_SUBSTRING,		"Text must start with specific text"},
		{RULES::STRING_ENDS_WITH_SUBSTRING,		"Text must end with specific text"},
		{RULES::NUMBER_IS_NOT_NEGATIVE,			"Number must not be negative"},
		{RULES::NUMBER_IS_NOT_ZERO,				"Number must not be zero"},
		{RULES::NUMBER_IS_NEGATIVE,				"Number must be negative"},
		{RULES::NUMBER_IS_GREATER_THAN,			"Number must be greater than specific number"},
		{RULES::NUMBER_IS_LESS_THAN,				"Number must be less than specific number"},
		{RULES::INTEGER_DIVISIBLE_BY_INTEGER,		"Whole number must be divisible by other whole number"},
		{RULES::CHAR_IS_ONE_OF_CHARACTER_SET,		"Character must be one of a set of characters"},
	};

	inline static const vector<string> stringRuleLabels_
	{
		validationRuleLabels_.at(RULES::ALL_PRESENCE),
		validationRuleLabels_.at(RULES::STRING_MAX_LENGTH),
		validationRuleLabels_.at(RULES::STRING_MIN_LENGTH),
		validationRuleLabels_.at(RULES::STRING_STARTS_WITH_SUBSTRING),
		validationRuleLabels_.at(RULES::STRING_ENDS_WITH_SUBSTRING)
	};

	inline static const vector<string> integerRuleLabels_
	{
		validationRuleLabels_.at(RULES::ALL_PRESENCE),
		validationRuleLabels_.at(RULES::NUMBER_IS_NOT_NEGATIVE),
		validationRuleLabels_.at(RULES::NUMBER_IS_NEGATIVE),
		validationRuleLabels_.at(RULES::NUMBER_IS_NOT_ZERO),
		validationRuleLabels_.at(RULES::NUMBER_IS_LESS_THAN),
		validationRuleLabels_.at(RULES::NUMBER_IS_GREATER_THAN),
		validationRuleLabels_.at(RULES::INTEGER_DIVISIBLE_BY_INTEGER)
	};

	inline static const vector<string> floatRuleLabels_
	{
		validationRuleLabels_.at(RULES::ALL_PRESENCE),
		validationRuleLabels_.at(RULES::NUMBER_IS_NOT_NEGATIVE),
		validationRuleLabels_.at(RULES::NUMBER_IS_NEGATIVE),
		validationRuleLabels_.at(RULES::NUMBER_IS_NOT_ZERO),
		validationRuleLabels_.at(RULES::NUMBER_IS_LESS_THAN),
		validationRuleLabels_.at(RULES::NUMBER_IS_GREATER_THAN)
	};

	inline static const vector<string> charRuleLabels_
	{
		validationRuleLabels_.at(RULES::ALL_PRESENCE),
		validationRuleLabels_.at(RULES::CHAR_IS_ONE_OF_CHARACTER_SET)
	};

public:
	static const string& GetValidationRuleLabel(const RULES& rule)
	{
		return validationRuleLabels_.at(rule);
	}//End GetValidationRuleLabelsOfType

	static const vector<string>& GetValidationRuleLabels(const DataType::DATA_TYPE type)
	{
		switch(type)
		{
			case DataType::DATA_TYPE::NONE: case DataType::DATA_TYPE::BOOLEAN: return {};
			case DataType::DATA_TYPE::STRING:	return stringRuleLabels_;
			case DataType::DATA_TYPE::INTEGER:	return integerRuleLabels_;
			case DataType::DATA_TYPE::FLOAT:	return floatRuleLabels_;
			case DataType::DATA_TYPE::CHAR:		return charRuleLabels_;
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