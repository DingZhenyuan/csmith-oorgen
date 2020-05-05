#ifndef KLEE_EXTENSION_H
#define KLEE_EXTENSION_H

#include <vector>
#include <ostream>
#include <string>
#include "AbsExtension.h"
#include "CVQualifiers.h"

class ExtensionValue;

class KleeExtension : public AbsExtension {
	friend class ExtensionMgr;
public:
	virtual void GenerateValues();

	virtual void OutputInit(std::ostream &out);

	virtual void OutputFirstFunInvocation(std::ostream &out, FunctionInvocation *invoke);

	virtual void OutputHeader(std::ostream &out);

	virtual void OutputTail(std::ostream &out);

	virtual std::vector<ExtensionValue *> &get_values() { return values_; }

private:

	static std::string input_base_name_;

	void output_symbolics(std::ostream &out);

	std::vector<ExtensionValue*> values_;

	KleeExtension();

	~KleeExtension();
};

#endif // KLEE_EXTENSION_H
