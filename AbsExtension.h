#ifndef ABS_EXTENSION_H
#define ABS_EXTENSION_H

#include <ostream>
#include <vector>

class Function;
class FunctionInvocation;
class ExtensionValue;

class AbsExtension {
	friend class ExtensionMgr;
public:
	AbsExtension();

	virtual ~AbsExtension() = 0;

	virtual void GenerateValues() = 0;

	virtual void OutputInit(std::ostream &out) = 0;

	virtual void OutputFirstFunInvocation(std::ostream &out, FunctionInvocation *invoke);

	virtual void OutputHeader(std::ostream &out) = 0;

	virtual void OutputTail(std::ostream &out) = 0;

	virtual std::vector<ExtensionValue *> &get_values() = 0;

	void Initialize(unsigned int num, std::vector<ExtensionValue *> &values);

	void GenerateFirstParameterList(Function &curfunc, std::vector<ExtensionValue *> &values);

	FunctionInvocation *MakeFuncInvocation(Function *func, std::vector<ExtensionValue *> &values);

protected:

	static const char *tab_;

	static const char *base_name_;

	void default_output_definitions(std::ostream &out, std::vector<ExtensionValue *> &values, bool init_flag);
};

#endif // ABS_EXTENSION_H
