#ifndef STATEMENT_GOTO_H
#define STATEMENT_GOTO_H

///////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include <string>
#include <map>
#include "Statement.h"
using namespace std;

class Expression;
class Block;
class Variable;
class CGContext;
class Statement;
/*
 *
 */
class StatementGoto : public Statement
{
public:
	// Factory method.
	static StatementGoto *make_random(CGContext &cg_context);

	StatementGoto(Block* b, const Expression &test, const Statement* dest, const std::vector<const Variable*>& vars);
	StatementGoto(const StatementGoto &sg);
	virtual ~StatementGoto(void);
	//
	static bool has_init_skipped_vars(const Block* src_blk, const Statement* dest);
	static Block* find_good_jump_block(vector<Block*>& blocks, const Block* blk, bool as_dest);

	virtual bool must_jump(void) const;
	virtual void get_exprs(std::vector<const Expression*>& exps) const {exps.push_back(&test);}
	virtual void get_blocks(std::vector<const Block*>& /* blks */) const {};
	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;
	virtual void Output(std::ostream &out, FactMgr* fm, int indent = 0) const;
	void output_skipped_var_inits(std::ostream &out, int indent) const;

	static void doFinalization(void);

	const Expression &test;
	const Statement* dest;
	std::string label;
	std::vector<const Variable*> init_skipped_vars;
	static std::map<const Statement*, std::string> stm_labels;
};

///////////////////////////////////////////////////////////////////////////////

#endif // STATEMENT_CONTINUE_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
