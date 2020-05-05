#ifndef EXPRESSION_FUNCALL_H
#define EXPRESSION_FUNCALL_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>

#include "Expression.h"
#include "FunctionInvocation.h"

class CGContext;
class FunctionInvocation;
class ExpressionVariable;

/*
 *
 */
class ExpressionFuncall : public Expression
{
public:
	// Factory method.
	static Expression *make_random(CGContext &cg_context, const Type* type, const CVQualifiers* qfer=0);

	explicit ExpressionFuncall(const FunctionInvocation &fi);

	virtual ~ExpressionFuncall(void);

	virtual Expression* clone() const;

	virtual CVQualifiers get_qualifiers(void) const;

	virtual void get_eval_to_subexps(vector<const Expression*>& subs) const {subs.push_back(this);}

	virtual const FunctionInvocation* get_invoke(void) const { return &invoke; };

	virtual const Type &get_type(void) const;

	virtual void get_called_funcs(std::vector<const FunctionInvocationUser*>& funcs ) const;

	virtual unsigned int get_complexity(void) const;

	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	virtual bool has_uncertain_call_recursive(void) const;

	virtual bool compatible(const Variable *) const;

	virtual bool compatible(const Expression *) const;

	virtual bool use_var(const Variable* v) const;

	virtual bool equals(int num) const { return invoke.equals(num);}
	virtual bool is_0_or_1(void) const { return invoke.is_0_or_1();}

	virtual std::vector<const ExpressionVariable*> get_dereferenced_ptrs(void) const;
	virtual void get_referenced_ptrs(std::vector<const Variable*>& ptrs) const;

	void Output(std::ostream &) const;
	virtual void indented_output(std::ostream &out, int indent) const;

private:
	const FunctionInvocation &invoke;

	explicit ExpressionFuncall(const ExpressionFuncall &efun);

	// unimplementable
	ExpressionFuncall &operator=(const ExpressionFuncall &ec);
};

///////////////////////////////////////////////////////////////////////////////

#endif // EXPRESSION_FUNCALL_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
