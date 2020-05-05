#ifndef LHS_H
#define LHS_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>

#include "Expression.h"

class CGContext;
class Variable;
class Constant;

/*
 *
 */
class Lhs : public Expression
{
public:
	// Factory method.
	static Lhs *make_random(CGContext &cg_context, const Type* t, const CVQualifiers* qfer, bool for_compound_assign, bool no_signed_overflow=false);

	explicit Lhs(const Variable &v);

	Lhs(const Variable &v, const Type* t, bool compound_assign);

	virtual ~Lhs(void);

	virtual Expression *clone() const;

	virtual CVQualifiers get_qualifiers(void) const;

	virtual void get_eval_to_subexps(vector<const Expression*>& subs) const {subs.push_back(this);}

	int get_indirect_level(void) const;

	void get_lvars(const vector<const Fact*>& facts, vector<const Variable*>& vars) const;

	bool is_volatile() const;

	const Variable* get_var(void) const {return &var;};

	bool compatible(const Variable *v) const;

	bool compatible(const Expression *c) const;

	bool visit_indices(vector<const Fact*>& inputs, CGContext& cg_context) const;
	//
	virtual std::vector<const ExpressionVariable*> get_dereferenced_ptrs(void) const;
	virtual void get_referenced_ptrs(std::vector<const Variable*>& ptrs) const;
	virtual unsigned int get_complexity(void) const { return 1;}

	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	virtual const Type &get_type(void) const;

	virtual void Output(std::ostream &) const;

private:
	explicit Lhs(const Lhs &lhs);

	bool ptr_modified_in_rhs(vector<const Fact*>& inputs, CGContext& cg_context) const;

	const Variable &var;

	const Type* type;

	const bool for_compound_assign;

	// unimplementable
	Lhs &operator=(const Lhs &ev);
};

///////////////////////////////////////////////////////////////////////////////

#endif // ELHS_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
