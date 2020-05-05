#ifndef COVERAGE_TEST_EXTENSION_H
#define COVERAGE_TEST_EXTENSION_H

#include <vector>
#include <ostream>
#include <string>
#include "AbsExtension.h"
#include "CVQualifiers.h"

class ExtensionValue;
class Constant;

class CoverageTestExtension : public AbsExtension {
	friend class ExtensionMgr;
public:
	virtual void GenerateValues();

	virtual void OutputInit(std::ostream &out);

	virtual void OutputFirstFunInvocation(std::ostream &out, FunctionInvocation *invoke);

	virtual void OutputHeader(std::ostream &out);

	virtual void OutputTail(std::ostream &out);

	virtual std::vector<ExtensionValue *> &get_values() { return values_; }

private:

	static std::string array_base_name_;

	static std::string array_index_;

	void output_array_init(std::ostream &out, int count);

	void output_decls(std::ostream &out);

	int inputs_size_;

	// used for tmp vars
	std::vector<ExtensionValue*> values_;

	// hold actual values passed to func_1
	std::vector<Constant*> test_values_;

	CoverageTestExtension(int input_size);

	~CoverageTestExtension();
};

#endif // COVERAGE_TEST_EXTENSION_H
