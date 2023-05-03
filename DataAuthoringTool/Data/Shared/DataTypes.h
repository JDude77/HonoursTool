#pragma once
#ifndef DATA_TYPES
#define DATA_TYPES

struct DataType
{
	enum struct DATA_TYPE
	{
		NONE,
		STRING,
		INTEGER,
		FLOAT,
		CHAR,
		BOOLEAN
	};

	inline static const char* typeLabels_[] =
	{
		"NO DATA TYPE SELECTED",
		"String (text)",
		"Integer (whole number)",
		"Float (floating-point number)",
		"Char (single-character)",
		"Boolean (true/false tickbox)"
	};

	static constexpr int typeLabelsCount_ = 6;
};
#endif