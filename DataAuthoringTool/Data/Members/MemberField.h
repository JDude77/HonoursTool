#pragma once
#ifndef MEMBER_FIELD
#define MEMBER_FIELD

#include "../Shared/DataInterfaces.h"

#include <memory>
using std::shared_ptr;

class Field;

class MemberField : public IDelete, public IValidate
{
public:
	int Validate() override;
	int Delete() override;

private:
	shared_ptr<Field> field_ = nullptr;
};
#endif