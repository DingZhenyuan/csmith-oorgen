#ifndef FUNCTION_H
#define FUNCTION_H

///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
using namespace std;

#include "Effect.h"
#include "Type.h"

class Block;
class Variable;
class FactMgr;
class Statement;
class CGContext;
class Fact;
class Constant;
class CVQualifiers;

///////////////////////////////////////////////////////////////////////////////

class Function
{
public:
	friend void GenerateFunctions(void);
	
	// 新的生成函数方法
	static void GenerateMemberFunction(int funcIndex, int funcNum);

	~Function();

	// Factory methods.
	static Function *make_first(void);
	// type = 0 means we don't care about the return type
	static Function *make_random(const CGContext& cg_context, const Type* type = 0, const CVQualifiers* qfer=0);
	// generate the signature, but not the body
	static Function* make_random_signature(const CGContext& cg_context, const Type* type, const CVQualifiers* qfer=0);

	static Function* choose_func(vector<Function *> funcs, const CGContext& cg_context, const Type* type, const CVQualifiers* qfer);

	static Function *get_one_function(const vector<Function *> &ok_funcs);

	static void doFinalization();
	static bool reach_max_functions_cnt();

	void generate_body_with_known_params(const CGContext &prev_context, Effect& effect_accum);
	void compute_summary(void);

	void Output(std::ostream &);
	void OutputForwardDecl(std::ostream &);

	bool is_built(void) const { return (build_state == BUILT); }
	bool need_return_stmt();
	bool is_effect_known(void) const { return (build_state == BUILT); }
	const Effect &get_feffect(void) const { return feffect; }

	void remove_irrelevant_facts(std::vector<const Fact*>& inputs) const;

	bool is_var_visible(const Variable* var, const Statement* stm) const;
	bool is_var_on_stack(const Variable* var, const Statement* stm) const;
	bool is_var_oos(const Variable* var, const Statement* stm) const;

	const std::vector<const Variable*>& get_referenced_ptrs(void) const { return referenced_ptrs;}
	bool is_pointer_referenced(void) { return !referenced_ptrs.empty();} const

	std::string name;
	std::vector<Variable*> param;
//	vector<Expression*> param_value;
	const Type* return_type;
//	bool isBackLink;

	Effect feffect;

	std::vector<Block*> stack;
	std::vector<Block*> blocks;
	Block *body;
	Constant *ret_c;

	Variable* rv;    // a dummy variable representing the return value
	std::vector<const Variable*> new_globals;  // collection of global variables created in this function
	std::vector<const Variable*> dead_globals; // collection of global variables that is dangling at the end of this function
	bool fact_changed;
	bool union_field_read;
	bool is_inlined;
	bool is_builtin;
	int  visited_cnt;
	Effect accum_eff_context;

private:
	static int deleteFunction(Function* func);

	Function(const std::string &name, const Type *return_type);
	Function(const std::string &name, const Type *return_type, bool is_builtin);
	void OutputHeader(std::ostream &);
	void OutputFormalParamList(std::ostream &);
	void GenerateBody(const CGContext& prev_context);
	void make_return_const();

	static void initialize_builtin_functions();
	static void make_builtin_function(const string &function_string);

private:
	enum { UNBUILT, BUILDING, BUILT } build_state;
	std::vector<const Variable*> referenced_ptrs;
};

void GenerateFunctions(void);
Function *GetFirstFunction(void);
// unsigned
long FuncListSize(void);

// 新方法，返回FuncList
vector<Function*> getFuncList();

void OutputForwardDeclarations(std::ostream &out);

// 新的输出函数声明的方法
void OutputForwardDeclarationsClass(std::ofstream &out_c);

void OutputFunctions(std::ostream &out);

// 新的输出函数体的方法
void OutputFunctionsClass(std::ofstream &out_c);
// 新的输出函数体的方法
void OutputFunctionsClass(int funcIndex, int funcNumPerClass, std::ofstream &out_c);

const std::vector<Function*>& get_all_functions(void);
FactMgr* get_fact_mgr_for_func(const Function* func);
FactMgr* get_fact_mgr(const CGContext* cg);
const Function* find_function_by_name(const string& name);
int find_function_in_set(const vector<const Function*>& set, const Function* f);
const Block* find_blk_for_var(const Variable* v);

///////////////////////////////////////////////////////////////////////////////

#endif // FUNCTION_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
