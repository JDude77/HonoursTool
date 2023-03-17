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
	ValidationFunction() = default;

	template<typename F>
	explicit ValidationFunction(F&& fun) : ValidationFunction(function(std::forward<F>(fun))) {}

	template<typename ... Args>
	explicit ValidationFunction(function<bool(Args...)> fun) : any_(fun) {}

	template<typename ... Args>
	bool operator()(Args&& ... args)
	{
		return std::invoke(std::any_cast<function<bool(Args...)>>(any_), std::forward<Args>(args)...);
	}//End () operator

private:
	std::any any_;
};

class ValidationRule
{
	//TODO: Refactor this. For the love of god, refactor ALL of this.
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

	const map<DataType::DATA_TYPE, vector<RULES>> rulesPerType_
	{
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

	const map<RULES, ValidationFunction> validationRuleFunctions_
	{
		{ALL_PRESENCE,					ValidationFunction(&Presence)},
		{STRING_MAX_LENGTH,				ValidationFunction(&StringMaxLength)},
		{STRING_MIN_LENGTH,				ValidationFunction(&StringMinLength)},
		{STRING_STARTS_WITH_SUBSTRING,	ValidationFunction(&StringStartsWithSubstring)},
		{STRING_ENDS_WITH_SUBSTRING,		ValidationFunction(&StringEndsWithSubstring)},
		{NUMBER_IS_NOT_NEGATIVE,			ValidationFunction(&NumberIsNotNegative)},
		{NUMBER_IS_NEGATIVE,				ValidationFunction(&NumberIsNegative)},
		{NUMBER_IS_NOT_ZERO,				ValidationFunction(&NumberIsNotZero)},
		{NUMBER_IS_GREATER_THAN,			ValidationFunction(&NumberIsGreaterThan)},
		{NUMBER_IS_LESS_THAN,				ValidationFunction(&NumberIsLessThan)},
		{INTEGER_DIVISIBLE_BY_INTEGER,	ValidationFunction(&IntegerDivisibleByInteger)},
		{CHAR_IS_ONE_OF_CHARACTER_SET,	ValidationFunction(&CharIsOneOfCharacterSet)},
	};

	//Need a map to serve as a dictionary like so:
	//DataType : List of validation rules that can be used for that type
	//Unsure whether to have list of enums which can be fed to something to invoke a function or just store the functions themselves
};


#endif