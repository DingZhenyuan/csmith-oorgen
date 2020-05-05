#ifndef FUNCTION_INVOCATION_UNARY_H
#define FUNCTION_INVOCATION_UNARY_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <string>

#include "FunctionInvocation.h"

class Type;
class CGContext;

class FunctionInvocationUnary : public FunctionInvocation
{
	friend class FunctionInvocation; /* XXX --- yuck! */

public:
	static FunctionInvocationUnary *CreateFunctionInvocationUnary(
				CGContext &cg_context,
				eUnaryOps op,
				SafeOpFlags *flags);

	virtual ~FunctionInvocationUnary(void);

	virtual FunctionInvocation* clone() const;

	virtual const Type &get_type(void) const;

	virtual bool compatible(const Variable *v) const;

	virtual void Output(std::ostream &) const;

	virtual void indented_output(std::ostream &out, int indent) const;

	virtual bool safe_invocation() const;

	virtual bool equals(int num) const;

	virtual bool is_0_or_1(void) const { return eFunc == eNot;}

private:
	eUnaryOps eFunc;

	std::string tmp_var;

private:
	FunctionInvocationUnary(eUnaryOps op, SafeOpFlags *flags, std::string &name);

	FunctionInvocationUnary(eUnaryOps op, SafeOpFlags *flags);

	explicit FunctionInvocationUnary(const FunctionInvocationUnary &funary);

	// unimplemented
	FunctionInvocationUnary &operator=(const FunctionInvocationUnary &fi);
};

///////////////////////////////////////////////////////////////////////////////

#endif // FUNCTION_INVOCATION_UNARY_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
