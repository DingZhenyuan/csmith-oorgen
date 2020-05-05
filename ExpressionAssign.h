#ifndef EXPRESSION_ASSIGN_H
#define EXPRESSION_ASSIGN_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include "Expression.h"
#include "StatementAssign.h"

class CGContext;
class ExpressionVariable;

/*
 *
 */
class ExpressionAssign : public Expression
{
public:
	// Factory method.
	static Expression *make_random(CGContext &cg_context, const Type* type, const CVQualifiers* qfer=0);

	ExpressionAssign(const StatementAssign* sa);
	virtual ~ExpressionAssign(void);

	virtual Expression *clone() const;

	virtual CVQualifiers get_qualifiers(void) const;

	virtual void get_eval_to_subexps(vector<const Expression*>& subs) const;

	virtual const Type &get_type(void) const { return assign->get_lhs()->get_type();}

	virtual void get_called_funcs(std::vector<const FunctionInvocationUser*>& funcs) const { assign->get_called_funcs(funcs);}

	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	virtual bool has_uncertain_call_recursive(void) const { return assign->has_uncertain_call_recursive();}

	virtual bool use_var(const Variable* v) const;

	virtual bool equals(int num) const;
	virtual bool is_0_or_1(void) const;

	virtual std::vector<const ExpressionVariable*> get_dereferenced_ptrs(void) const { return assign->get_dereferenced_ptrs();}
	virtual void get_referenced_ptrs(std::vector<const Variable*>& ptrs) const { assign->get_referenced_ptrs(ptrs);}
	virtual unsigned int get_complexity(void) const { return get_lhs()->get_complexity() + get_rhs()->get_complexity();}

	const Expression* get_rhs(void) const { return assign->get_expr();}
	const Lhs* get_lhs(void) const { return assign->get_lhs();}
	const StatementAssign* get_stm_assign(void) const { return assign;}
	void Output(std::ostream &) const;
	virtual void indented_output(std::ostream &out, int indent) const;

private:
	const StatementAssign* assign;
};

///////////////////////////////////////////////////////////////////////////////

#endif // EXPRESSION_ASSIGN_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
