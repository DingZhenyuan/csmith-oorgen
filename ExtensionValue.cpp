#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "ExtensionValue.h"
#include <vector>
#include "Type.h"
#include "Constant.h"

ExtensionValue::ExtensionValue(const Type *type, string name)
	: type_(type),
	  value_(NULL),
	  name_(name)
{
	vector<bool> isConsts;
	vector<bool> isVolatiles;
	isConsts.push_back(false);
	isVolatiles.push_back(false);
	qfer_ = CVQualifiers(isConsts, isVolatiles);
}

ExtensionValue::~ExtensionValue()
{
	delete value_;
}

