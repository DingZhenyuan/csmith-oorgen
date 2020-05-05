#ifndef SPLAT_EXTENSION_H
#define SPLAT_EXTENSION_H

#include <vector>
#include <ostream>
#include <string>
#include "AbsExtension.h"
#include "CVQualifiers.h"

class ExtensionValue;

class SplatExtension : public AbsExtension {
	friend class ExtensionMgr;
public:
	virtual void GenerateValues();

	virtual void OutputInit(std::ostream &out);

	virtual void OutputFirstFunInvocation(std::ostream &out, FunctionInvocation *invoke);

	virtual void OutputHeader(std::ostream &out);

	virtual void OutputTail(std::ostream &out);

	virtual std::vector<ExtensionValue *> &get_values() { return values_; }

private:

	void output_symbolics(std::ostream &out);

	std::vector<ExtensionValue*> values_;

	SplatExtension();

	~SplatExtension();
};

#endif // SPLAT_EXTENSION_H
