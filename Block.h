#ifndef BLOCK_H
#define BLOCK_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <vector>
#include <map>

#include "Statement.h"
#include "Type.h"

class CGContext;
class Statement;
class Variable;
class Fact;
class FactMgr;
class Effect;

/*
 *
 */
class Block : public Statement
{
public:
	// Factory method.
	static Block *make_random(CGContext &cg_context, bool looping = false);

	static Block *make_dummy_block(CGContext &cg_context);

	Block(Block* b, int block_size);
	virtual ~Block(void);

	//
	virtual bool set_depth_protect(bool b) { depth_protect = b; return b; }
	virtual bool get_depth_protect(void) { return depth_protect; }

	virtual void Output(std::ostream &out, FactMgr* fm, int indent=0) const;

	void OutputTmpVariableList(std::ostream &out, int indent) const;

    Block* random_parent_block(void);

	int block_size() { return block_size_; }
	// These are currently accessed directly.
	std::vector<Statement *> stms;
	std::vector<Statement *> deleted_stms;
	std::vector<Variable *> local_vars;
	mutable std::map<std::string, enum eSimpleType> macro_tmp_vars;

	std::string create_new_tmp_var(enum eSimpleType type) const;

	virtual void get_blocks(std::vector<const Block*>& blks) const { blks.push_back(this);}
	virtual void get_exprs(std::vector<const Expression*>& /* exps */) const {};
	const Statement* get_last_stm(void) const;
	bool is_var_on_stack(const Variable* v) const;
	std::vector<const ExpressionVariable*> get_dereferenced_ptrs(void) const;

	Statement* append_return_stmt(CGContext& cg_context);
	virtual bool must_return(void) const;
	bool must_break_or_return(void) const;
	virtual bool must_jump(void) const;
	bool from_tail_to_head(void) const;
	bool need_nested_loop(const CGContext& cg_context);
	Statement* append_nested_loop(CGContext& cg_context);

	virtual bool visit_facts(vector<const Fact*>& inputs, CGContext& cg_context) const;

	bool contains_back_edge(void) const;

	bool find_fixed_point(vector<const Fact*> inputs, vector<const Fact*>& post_facts, CGContext& cg_context, int& fail_index, bool visit_once) const;

	void post_creation_analysis(CGContext& cg_context, const Effect& pre_effect);

	size_t remove_stmt(const Statement* s);

	bool looping;

	bool in_array_loop;

	bool need_revisit;

	std::vector<const Statement*> break_stms;

private:

	bool depth_protect;

	// maximum block size
	const int block_size_;

	void set_accumulated_effect(CGContext& cg_context) const;

	//////////////////////////////////////////////////////////////
	Block(const Block &b); // unimplemented

	Block &operator=(const Block &b); // unimplemented
};

Block* find_block_by_id(int blk_id);
///////////////////////////////////////////////////////////////////////////////

#endif // BLOCK_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
