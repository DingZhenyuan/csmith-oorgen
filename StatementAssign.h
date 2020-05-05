#ifndef STATEMENT_ASSIGN_H
#define STATEMENT_ASSIGN_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <vector>
#include "Statement.h"
#include "FunctionInvocationBinary.h"
#include "Probabilities.h"
#include "Lhs.h"

class CGContext;
class Expression;
class Variable;
class SafeOpFlags;
class FactMgr;
class Lhs;
template <class Key, class Value>
class ProbabilityTable;

/*
 * "Assignment operators."  This is sort of a dumb model, but it will do for
 * now.
 */
enum eAssignOps
{
	eSimpleAssign,
	eMulAssign,
	eDivAssign,
	eRemAssign,
	eAddAssign,
	eSubAssign,
	eLShiftAssign,
	eRShiftAssign,
	eBitAndAssign,
	eBitXorAssign,
	eBitOrAssign,
	ePreIncr,
	ePreDecr,
	ePostIncr,
	ePostDecr
};
#define MAX_ASSIGN_OP ((eAssignOps) (ePostDecr+1))

/*
 *
 */
class StatementAssign : public Statement
{
public:
	// Factory method.
	static StatementAssign *make_random(CGContext &cg_context, const Type* type=0, const CVQualifiers* qfer=0);

	static StatementAssign *make_possible_compound_assign(CGContext &cg_context,
						 const Type *type,
						 const Lhs &l,
						 eAssignOps op,
						 const Expression &e);

	StatementAssign(Block* b, const Lhs &l, const Expression &e, eAssignOps op = eSimpleAssign, const SafeOpFlags *flags = NULL);

	StatementAssign(Block* b, const Lhs &l, eAssignOps op, const Expression &e,
			const Expression *er, const SafeOpFlags *flags,
			std::string &tmp_name1, std::string &tmp_name2);

	static void InitProbabilityTable();
	static bool safe_assign(eAssignOps op);
	static bool need_no_rhs(eAssignOps op) { return op==ePreIncr || op==ePreDecr || op==ePostIncr || op==ePostDecr;}

	bool is_simple_assign(void) const { return op == eSimpleAssign;}

	const Lhs* get_lhs(void) const {return &lhs;};
	const Expression* get_rhs(void) const { return rhs;};
	const Expression* get_expr(void) const { return &expr;};

	static eAssignOps number_to_op(unsigned int value);
	// StatementAssign canonize();

	static eBinaryOps compound_to_binary_ops(eAssignOps op);

	static bool AssignOpWorksForFloat(eAssignOps op);

	virtual ~StatementAssign(void);

	virtual void get_blocks(std::vector<const Block*>& /* blks */) const {};
	virtual void get_exprs(std::vector<const Expression*>& exps) const {exps.push_back(&expr); exps.push_back(&lhs);}
	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	virtual std::vector<const ExpressionVariable*> get_dereferenced_ptrs(void) const;
	virtual bool has_uncertain_call_recursive(void) const;

	virtual void Output(std::ostream &out, FactMgr* fm, int indent = 0) const;
	void output_op(std::ostream &out) const;

	// XXX --- This should go away, once assignments are properly modeled as
	// expressions.
	virtual void OutputAsExpr(std::ostream &out) const;

	void OutputSimple(std::ostream &out) const;

private:
	static eAssignOps AssignOpsProbability(const Type* type);

	const eAssignOps op;
	const Lhs   &lhs;
	const Expression &expr;
	// the real rhs canonized from compound operators, for example rhs for "i += 1" is "i + 1"
	const Expression* rhs;
	const SafeOpFlags *op_flags;
	// used for AssignAdd etc;
	std::string tmp_var1;
	std::string tmp_var2;

	static DistributionTable assignOpsTable_;

	StatementAssign(const StatementAssign &sa);  // unimplemented

	StatementAssign &operator=(const StatementAssign &se); // unimplementable
};

///////////////////////////////////////////////////////////////////////////////

#endif // STATEMENT_ASSIGN_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
