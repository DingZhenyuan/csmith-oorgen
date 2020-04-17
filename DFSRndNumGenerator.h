#ifndef DFS_RNDNUM_GENERATOR_H
#define DFS_RNDNUM_GENERATOR_H

#include <vector>
#include <string>

#include "Common.h"
#include "CommonMacros.h"
#include "AbsRndNumGenerator.h"

class Sequence;
class Filter;

class DFSRndNumGenerator : public AbsRndNumGenerator
{
public:
	virtual ~DFSRndNumGenerator();
	// 初始化生成器impl_,
	static DFSRndNumGenerator *make_rndnum_generator();
	// 将前缀+name输出到ss
	virtual std::string get_prefixed_name(const std::string &name);
	// 返回trace_string_用于debug
	virtual std::string& trace_depth();
	// 获取ss中的值对到sequenc中
	virtual void get_sequence(std::string &sequence);
	// 随机生成不大于n的数
	virtual unsigned int rnd_upto(const unsigned int n, const Filter *f = NULL, const std::string *where = NULL);
	// 根据p概率返回true，对p=100和0都计入invalid
	virtual bool rnd_flipcoin(const unsigned int p, const Filter *f = NULL, const std::string *where = NULL);
	// 生成16进制数
	virtual std::string RandomHexDigits( int num );
	// 生成10进制数
	virtual std::string RandomDigits( int num );

	virtual enum RNDNUM_GENERATOR kind() { return rDFSRndNumGenerator; }
	// 从DepthSpec.cpp调用。返回true表示我们渴望回溯。
	bool eager_backtracking(int depth_needed);

	int get_decision_depth() { return decision_depth_; }
	// 重置，将current_pos和trace_string_还原，seq_清空
	void reset_state(void);

	int get_current_pos(void) { return current_pos_; }
	void set_current_pos(int pos) { current_pos_ = pos; }
	bool get_all_done(void) { return all_done_; }

private:
	// 嵌套类SearchState的前向声明
	class SearchState;

	DFSRndNumGenerator(Sequence *concrete_seq);
	// 对state再次检测，并将值对信息加入到seq_中
	int revisit_node(SearchState *state, int local_current_pos,
						int bound, const Filter *f, const string *where);
	// 对seq_初始化，加入bound为0的数
	void initialize_sequence(const std::vector<int> &v);

	void dumpCurrentState(int bound, const std::string &where);

	virtual unsigned long genrand(void);
	// 检测num是否属于invalid_nums
	bool filter_invalid_nums(vector<int> *invalid_nums, int v);
	// ???
	int random_choice(int bound, const Filter *f = NULL, const std::string *where = NULL, std::vector<int> *invalid_nums = NULL);
	// 新建size个state,存入states_中
	void init_states(int size);
	// 将位置和深度记录到trace_string中
	void log_depth(int d, const std::string *where = NULL, const char *log = NULL);

	// ----------------------------------------------------------------------------------------
	static DFSRndNumGenerator *impl_;

	//static std::string name_prefix;

	std::string trace_string_;

	// The current decision depth which should be less than SEARCH_STATE_SIZE
	// It represents the current depth where we are making random choices
	int decision_depth_;

	// The current position at states.
	int current_pos_;

	bool all_done_;

	Sequence *seq_;

	bool use_debug_sequence_;

	// Holds the vector representation of all DFS nodes.
	std::vector<SearchState*> states_;

	// disallow copy and assignment constructors
	DISALLOW_COPY_AND_ASSIGN(DFSRndNumGenerator);
};

#endif //DFS_RNDNUM_GENERATOR_H
