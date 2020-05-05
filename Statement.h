#ifdef WIN32
#pragma warning(disable : 4786)   /* Disable annoying warning messages */
#endif
#include <vector>
#include <ostream>
#include <string>
#include "Probabilities.h"
using namespace std;

#ifndef STATEMENT_H
#define STATEMENT_H

///////////////////////////////////////////////////////////////////////////////

class CGContext;
class Function;
class FunctionInvocation;
class FunctionInvocationUser;
class ExpressionVariable;
class FactMgr;
class Fact;
class Block;
class Effect;
class CFGEdge;

template <class Key, class Value>
class ProbabilityTable;
class StatementGoto;
class Variable;
class Expression;

enum eStatementType
{
	eAssign,
	eBlock,
	eFor,		// Make this a generic loop construct (while/for/do)
	eIfElse,
	eInvoke,
	eReturn,
	eContinue,
	eBreak,
	eGoto,
	eArrayOp
//  ..more?  try, catch, throw
//	eHash,
};
#define MAX_STATEMENT_TYPE ((eStatementType) (eArrayOp+1))

/*
 *
 */
class Statement
{
public:
	// Factory method.
	static Statement *make_random(CGContext &cg_context,
								  eStatementType t = MAX_STATEMENT_TYPE);
	static eStatementType number_to_type(unsigned int value);

	virtual ~Statement(void);

	eStatementType get_type(void) const { return eType; }

	void get_called_funcs(std::vector<const FunctionInvocationUser*>& funcs) const;

	const FunctionInvocation* get_direct_invocation(void) const;

	virtual bool visit_facts(vector<const Fact*>& /*inputs*/, CGContext& /*cg_context*/) const {return true;};

	void output_hash(std::ostream &out, int indent) const;

	bool stm_visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	bool validate_and_update_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	int shortcut_analysis(vector<const Fact*>& inputs, CGContext& cg_context) const;

	bool analyze_with_edges_in(vector<const Fact*>& inputs, CGContext& cg_context) const;

	int find_typed_stmts(vector<const Statement*>& stms, const vector<int>& stmt_types) const;

	bool contains_stmt(const Statement* s) const;

	int find_contained_labels(vector<string>& labels) const;

	bool contains_unfixed_goto(void) const;

	void post_creation_analysis(vector<const Fact*>& pre_facts, const Effect& pre_effect,  CGContext& cg_context) const;

	void add_back_return_facts(FactMgr* fm, std::vector<const Fact*>& facts) const;

	bool in_block(const Block* b) const;

	bool dominate(const Statement* s) const;

	int find_edges_in(vector<const CFGEdge*>& edges, bool post_stm, bool back_link) const;

	bool has_edge_in(bool post_dest, bool back_link) const;

	const Statement* find_container_stm(void) const;

	static bool is_compound(eStatementType t) {return t==eBlock || t==eFor || t==eIfElse || t==eArrayOp;}

	bool is_ctrl_stmt(void) const {return eType == eContinue || eType == eBreak || eType == eGoto;}

	bool is_1st_stm(void) const;

	bool is_jump_target_from_other_blocks(void) const;

	bool read_union_field(void) const;

	virtual void get_blocks(std::vector<const Block*>& /* blks */) const = 0;
	virtual void get_exprs(std::vector<const Expression*>& /* exps */) const = 0;

	std::string find_jump_label(void) const;
	int find_jump_sources(std::vector<const StatementGoto*>& gotos) const;

	void set_accumulated_effect_after_block(Effect& eff, const Block* b, CGContext& cg_context) const;

	virtual bool has_uncertain_call_recursive(void) const {return false;}

	virtual bool must_return(void) const {return false;}

	virtual bool must_jump(void) const {return false;}

	virtual std::vector<const ExpressionVariable*> get_dereferenced_ptrs(void) const;

	void get_referenced_ptrs(std::vector<const Variable*>& ptrs) const;
	bool is_ptr_used(void) const;

	virtual void Output(std::ostream &out, FactMgr* fm=0, int indent = 0) const = 0;
	int pre_output(std::ostream &out, FactMgr* fm=0, int indent = 0) const;
	void post_output(std::ostream &out, FactMgr* fm=0, int indent = 0) const;

	void output_with_assert(std::ostream &out);

	const eStatementType eType;

	static int get_current_sid(void) { return sid; }

	int get_blk_depth(void) const;

	// unique id for each statement
	int stm_id;
	Function* func;
	Block* parent;
	static const Statement* failed_stm;

	static ProbabilityTable<unsigned int, ProbName> *stmtTable_;
protected:
	Statement(eStatementType st, Block* parent);

private:
	static int sid;

	Statement &operator=(const Statement &s); // unimplementable

	static void InitProbabilityTable();
};

int find_stm_in_set(const vector<const Statement*>& set, const Statement* s);

///////////////////////////////////////////////////////////////////////////////

#endif // STATEMENT_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
