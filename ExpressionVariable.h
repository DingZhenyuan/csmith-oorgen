#ifndef EXPRESSION_VARIABLE_H
#define EXPRESSION_VARIABLE_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <vector>
#include "Expression.h"
using namespace std;

class CGContext;
class Variable;

/*
 *
 */
class ExpressionVariable : public Expression
{
public:
	// Factory method.
	static ExpressionVariable *make_random(CGContext &cg_context, const Type* type, const CVQualifiers* qfer=0, bool as_param=false, bool as_return=false);

	explicit ExpressionVariable(const Variable &v);

	ExpressionVariable(const Variable &v, const Type* t);

	virtual ~ExpressionVariable(void);

	virtual Expression* clone() const;

	virtual CVQualifiers get_qualifiers(void) const;

	virtual void get_eval_to_subexps(vector<const Expression*>& subs) const {subs.push_back(this);}

	int get_indirect_level(void) const;

	const Variable* get_var(void) const {return &var;};
	//
	virtual std::vector<const ExpressionVariable*> get_dereferenced_ptrs(void) const;
	virtual void get_referenced_ptrs(std::vector<const Variable*>& ptrs) const;

	virtual unsigned int get_complexity(void) const { return 1;}

	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	virtual const Type &get_type(void) const;

	virtual bool compatible(const Expression *exp) const;

	virtual bool compatible(const Variable *v) const;

	virtual bool use_var(const Variable* v) const { return v == &var;}

	virtual void Output(std::ostream &) const;

private:
	explicit ExpressionVariable(const ExpressionVariable &expr);

	const Variable &var;

	const Type* type;

	// unimplementable
	ExpressionVariable &operator=(const ExpressionVariable &ev);
};

///////////////////////////////////////////////////////////////////////////////

#endif // EXPRESSION_VARIABLE_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
