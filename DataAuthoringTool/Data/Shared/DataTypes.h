#pragma once
#ifndef DATA_TYPES
#define DATA_TYPES

static struct DataType
{
	enum DATA_TYPE
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

	static constexpr int typeLabelsCount = 6;
};
#endif