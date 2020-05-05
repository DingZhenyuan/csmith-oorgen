#ifndef EXTENSION_VALUE_H
#define EXTENSION_VALUE_H

#include <string>
#include "CVQualifiers.h"

class Type;
class Constant;

class ExtensionValue {
public:
	ExtensionValue(const Type *type, std::string name);

	~ExtensionValue();

	const Type *get_type(void) { return type_; }

	Constant *get_value(void) { return value_; }

	const std::string &get_name(void) { return name_; }

	CVQualifiers get_qfer() { return qfer_; }

	void set_value(Constant *value) { value_ = value; }

private:
	const Type *type_;

	Constant *value_;

	const std::string name_;

	CVQualifiers qfer_;
};

#endif // EXTENSION_VALUE_H
