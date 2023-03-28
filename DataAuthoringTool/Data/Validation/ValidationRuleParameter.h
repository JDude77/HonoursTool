#pragma once
#ifndef VALIDATION_RULE_PARAMETER
#define VALIDATION_RULE_PARAMETER

class ValidationRuleParameter
{
public:
	static constexpr int stringMaxLengthBufferSize_ = 10;
	static constexpr int stringMinLengthBufferSize_ = 10;
	static constexpr int stringStartsWithSubstringBufferSize_ = 64;
	static constexpr int stringEndsWithSubstringBufferSize_ = 64;
	static constexpr int numberIsGreaterThanBufferSize_ = 11;
	static constexpr int numberIsLessThanBufferSize_ = 11;
	static constexpr int integerDivisibleByIntegerBufferSize_ = 11;
	static constexpr int charIsOneOfCharacterSetBufferSize_ = 32;


	char* GetBuffer(const RULES rule)
	{
		switch(rule)
		{
			case STRING_MAX_LENGTH:				return stringMaxLengthBuffer_;
			case STRING_MIN_LENGTH:				return stringMinLengthBuffer_;
			case STRING_STARTS_WITH_SUBSTRING:	return stringStartsWithSubstringBuffer_;
			case STRING_ENDS_WITH_SUBSTRING:	return stringEndsWithSubstringBuffer_;
			case NUMBER_IS_LESS_THAN:			return numberIsLessThanBuffer_;
			case NUMBER_IS_GREATER_THAN:		return numberIsGreaterThanBuffer_;
			case INTEGER_DIVISIBLE_BY_INTEGER:	return integerDivisibleByIntegerBuffer_;
			case CHAR_IS_ONE_OF_CHARACTER_SET:	return charIsOneOfCharacterSetBuffer_;

			case NUMBER_IS_NOT_NEGATIVE:
			case NUMBER_IS_NOT_ZERO:
			case NUMBER_IS_NEGATIVE:
			case ALL_PRESENCE:
			case NA:
			default: return nullptr;
		}//End switch
	}//End GetBuffer

	int* GetBufferCurrentLength(const RULES rule)
	{
		switch(rule)
		{
			case STRING_MAX_LENGTH:				return &stringMaxLengthBufferCurrentLength_;
			case STRING_MIN_LENGTH:				return &stringMinLengthBufferCurrentLength_;
			case STRING_STARTS_WITH_SUBSTRING:	return &stringStartsWithSubstringBufferCurrentLength_;
			case STRING_ENDS_WITH_SUBSTRING:	return &stringEndsWithSubstringBufferCurrentLength_;
			case NUMBER_IS_LESS_THAN:			return &numberIsLessThanBufferCurrentLength_;
			case NUMBER_IS_GREATER_THAN:		return &numberIsGreaterThanBufferCurrentLength_;
			case INTEGER_DIVISIBLE_BY_INTEGER:	return &integerDivisibleByIntegerBufferCurrentLength_;
			case CHAR_IS_ONE_OF_CHARACTER_SET:	return &charIsOneOfCharacterSetBufferCurrentLength_;

			case NUMBER_IS_NOT_NEGATIVE:
			case NUMBER_IS_NOT_ZERO:
			case NUMBER_IS_NEGATIVE:
			case ALL_PRESENCE:
			case NA:
			default: return nullptr;
		}//End switch
	}//End GetBufferCurrentLength

private:
	char stringMaxLengthBuffer_[stringMaxLengthBufferSize_] = {};
	int stringMaxLengthBufferCurrentLength_ = 0;

	char stringMinLengthBuffer_[stringMinLengthBufferSize_] = {};
	int stringMinLengthBufferCurrentLength_ = 0;

	char stringStartsWithSubstringBuffer_[stringStartsWithSubstringBufferSize_] = {};
	int stringStartsWithSubstringBufferCurrentLength_ = 0;

	char stringEndsWithSubstringBuffer_[stringEndsWithSubstringBufferSize_] = {};
	int stringEndsWithSubstringBufferCurrentLength_ = 0;

	char numberIsGreaterThanBuffer_[numberIsGreaterThanBufferSize_] = {};
	int numberIsGreaterThanBufferCurrentLength_ = 0;

	char numberIsLessThanBuffer_[numberIsLessThanBufferSize_] = {};
	int numberIsLessThanBufferCurrentLength_ = 0;

	char integerDivisibleByIntegerBuffer_[integerDivisibleByIntegerBufferSize_] = {};
	int integerDivisibleByIntegerBufferCurrentLength_ = 0;

	char charIsOneOfCharacterSetBuffer_[charIsOneOfCharacterSetBufferSize_] = {};
	int charIsOneOfCharacterSetBufferCurrentLength_ = 0;
};
#endif