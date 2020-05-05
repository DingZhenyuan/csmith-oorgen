#ifndef REDUCER_H
#define REDUCER_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "Effect.h"
#include "util.h"
using namespace std;

class Function;
class Block;
class Statement;
class Variable;
class Expression;
class ExpressionVariable;
class FactMgr;
class Type;
class FunctionInvocation;
class FunctionInvocationUser;
class FunctionInvocationBinary;

/*
 *
 */
class Reducer
{
public:
	Reducer(string fname);
	virtual ~Reducer(void);
	void configure(void);

	bool is_blk_deleted(const Block* b) const;
	bool is_stm_deleted(const Statement* stm) const;
	bool is_var_used(const Variable* v);
	bool is_param_dropped(const Function* f, int i);
	bool is_replaced_var(const ExpressionVariable* ev, string& str_out);
	const Expression* get_replaced_invocation(const FunctionInvocation* fi);
	bool is_ptr_written_in_stm(const Statement* stm);
	bool is_exp_replaced(const Expression* e);
	bool is_label_used(const string l);
	bool is_var_init_reduced(const Variable* v);
	string find_jump_label(const Statement* stm);
	int find_required_labels(const Statement* stm, vector<string>& labels);
	int find_missing_labels(const Statement* stm, const Statement* alt_stm, vector<string>& labels);

	// for effect analysis and variable dropping
	vector<const Variable*> find_used_vars(string assigns);
	const Variable* find_addressed_var(string addr);
	void find_called_funcs(const FunctionInvocation* fi, string id, vector<const FunctionInvocationUser*>& funcs, vector<string>& ids);
	void get_used_vars_and_funcs_and_labels(const FunctionInvocation* fi, vector<const Variable*>& vars, vector<const Function*>& funcs, vector<string>& labels);
	void get_used_vars_and_funcs_and_labels(const Expression* e, vector<const Variable*>& vars, vector<const Function*>& funcs, vector<string>& labels);
	void get_used_vars_and_funcs_and_labels(const Statement* stm, vector<const Variable*>& vars, vector<const Function*>& funcs, vector<string>& labels);
	void expand_used_vars(void);
	const FunctionInvocation* find_invoke_by_eid(const Statement* s, int id) const;
	const FunctionInvocation* find_invoke_by_eid(const Expression* e, int id) const;
	int find_binary_operations(const Statement* stm, vector<const FunctionInvocationBinary*>& ops, vector<int>& ids, bool no_ptr_cmp=false);
	int find_binary_operations(const Expression* exp, vector<const FunctionInvocationBinary*>& ops, vector<int>& ids, bool no_ptr_cmp=false);
	void build_left_right_binary_trees(vector<const FunctionInvocationBinary*>& ops, vector<intvec>& left_trees, vector<intvec>& right_trees);
	int  reduce_const_binary_op(const FunctionInvocationBinary* fib);
	void reduce_const_binary_ops(vector<const FunctionInvocationBinary*>& ops);

	// for function dropping
	int reduce_call_with_assigns(const string& tmp_name, const string& value, const string& extra_assigns);

	// for statement reduction
	void replace_stm(const Statement* stm, const Statement* new_stm, string pre_stm);
	void delete_stms_after(const Statement* stm, bool include_parent_blks);
	int reduce_stms_with_assigns(int id1, int id2, const string& assigns);

	string add_artificial_globals(const Type* t, string name="");

	// for outputing
	void output_block_skeleton(const Block* blk, vector<const Block*>& work_blks, vector<const Function*>& work_funcs, std::ostream& out);
	int  output_expr(const Expression* e, std::ostream &out);

	// for interaction with reduction config file
	void config_active_blks(string cmd);
	int  configure_diff_active_blks(string line, int first_bid);
	void config_call_chain_shortcut(string cmd);
	void config_stm_reduction(string cmd);
	void config_expr_reduction(string cmd);
	void config_binary_reduction(string cmd);
	void config_if_reduction(string cmd);
	void config_var_init_reduction(string cmd);

	bool dump_block_entry;
	bool dump_all_block_info;
	bool dump_monitored_var;
	bool dump_dropped_params;
	bool drop_params;
	const Function* rewrite_calls_inside;
	bool reduce_binaries;
	bool output_if_ids;
	const Variable* monitored_var;
	const Function* monitored_func;
	string monitored_call_id;
	string crc_lines;

	std::map<const Block*, int> map_active_blks;
	vector<int> all_blks;
	vector<const Variable*> dropped_params;

	// for statement reduction
	std::map<const Statement*, const Statement*> replaced_stms;
	std::vector<const Statement*> dump_value_before;
	std::vector<const Statement*> dump_value_after;
	std::map<string, vector<const Variable*> > map_str_effects;
	const Function* dump_stms_in_blocks;

	// for expression reduction
	std::map<const Expression*, string> map_reduced_vars;
	std::map<const FunctionInvocation*, const Expression*> map_reduced_invocations;
	std::map<const Statement*, string> map_pre_stm_assigns;
	std::map<const Variable*, string> map_reduced_var_inits;
	std::vector<const FunctionInvocation*> must_use_var_invocations;
	std::vector<const Statement*> must_use_var_stms;

	vector<const Function*> used_funcs;
	vector<const Variable*> used_vars;
	vector<string> used_labels;
	vector<string> artificial_globals;
	const Function* main;
	string main_str;
	bool configured;

private:
	string fname_;
	const Statement* find_stm_by_id(int stm_id);
	int find_local_vars_to_lift(vector<const Variable*>& vars);
};

///////////////////////////////////////////////////////////////////////////////

#endif // REDUCER_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
