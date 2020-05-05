#ifndef EXTENSION_MGR_H
#define EXTENSION_MGR_H

#include <ostream>

class AbsExtension;
class Function;
class FunctionInvocation;
class CGContext;

class ExtensionMgr {
public:
	static void CreateExtension();

	static void DestroyExtension();

	static AbsExtension *GetExtension();

	static void GenerateFirstParameterList(Function &curFunc);

	static void GenerateValues();

	static FunctionInvocation *MakeFuncInvocation(Function *curFunc, CGContext &cg_context);

	static void OutputTail(std::ostream &out);

	static void OutputHeader(std::ostream &out);

	static void OutputInit(std::ostream &out);

	static void OutputFirstFunInvocation(std::ostream &out, FunctionInvocation *invoke);

private:
	static AbsExtension *extension_;

};

#endif // EXTENSION_MGR_H
