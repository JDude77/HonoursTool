#pragma once
#ifndef FIELD_TYPE
#define FIELD_TYPE
struct Field
{
	//TODO: Make this more dynamic to optimise for storage space and allow for larger bodies of text if need be
	constexpr static int maxDataBufferSize_ = 512;
	char dataBuffer_[maxDataBufferSize_]{};
	int dataBufferCurrentSize_ = 0;
	bool booleanData = false;
};
#endif