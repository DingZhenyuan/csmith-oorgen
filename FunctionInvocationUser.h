#ifndef FUNCTION_INVOCATION_USER_H
#define FUNCTION_INVOCATION_USER_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <vector>
#include "FunctionInvocation.h"
#include "Type.h"
#include "Fact.h"

class CGContext;
class Function;
class Statement;
class Variable;
class SafeOpFlags;
class Variable;

class FunctionInvocationUser: public FunctionInvocation
{
	friend class FunctionInvocation;

public:
	// factory method
	static FunctionInvocationUser* build_invocation_and_function(CGContext &cg_context, const Type* type, const CVQualifiers* qfer);

	virtual ~FunctionInvocationUser(void);

	virtual FunctionInvocation* clone() const;

	virtual bool compatible(const Variable *) const { return false; }

	virtual const Type &get_type(void) const;

	virtual void Output(std::ostream &) const;

	virtual void indented_output(std::ostream &out, int indent) const;

	virtual bool safe_invocation() const { return true; }

	const Function* get_func(void) const { return func; };

	bool revisit(std::vector<const Fact*>& inputs, CGContext& cg_context) const;

	void save_return_fact(const vector<const Fact*>& facts) const;

	static void doFinalization(void);

	FunctionInvocationUser(Function *target, bool isBackLink, const SafeOpFlags *flags);

private:
	Function *func;
	bool isBackLink;

private:
	explicit FunctionInvocationUser(const FunctionInvocationUser &fiu);

	// unimplemented
	//FunctionInvocationUser &operator=(const FunctionInvocationUser &fi);

	bool build_invocation(Function *target, CGContext &cg_context);
};

const Fact* get_return_fact_for_invocation(const FunctionInvocationUser* fiu, const Variable* var, enum eFactCategory cat);

void calls_to_funcs(const vector<const FunctionInvocationUser*>& calls, vector<const Function*>& funcs);
void calls_to_funcs_recursive(const vector<const FunctionInvocationUser*>& calls, vector<const Function*>& funcs);

///////////////////////////////////////////////////////////////////////////////

#endif // FUNCTION_INVOCATION_USER_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
