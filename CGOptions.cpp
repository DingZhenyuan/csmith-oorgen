#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <iostream>
#include <cassert>
#include <cstring>
#include <map>

#include "CGOptions.h"
#include "StringUtils.h"

using namespace std;

Reducer* CGOptions::reducer_ = NULL;
vector<int> CGOptions::safe_math_wrapper_ids_;
map<string, bool> CGOptions::enabled_builtin_kinds_;
int CGOptions::int_size_ = 0;
int CGOptions::pointer_size_ = 0;


// 定义各种类型的变量的setters和getters
#define DEFINE_GETTER_SETTER(type, init, f) \
    type CGOptions::f##_ = (init); \
    type CGOptions::f(void) { return f##_; } \
    type CGOptions::f(type p) { f##_ = p; return p; }

// bool, 默认false
#define DEFINE_GETTER_SETTER_BOOL(f) \
    DEFINE_GETTER_SETTER(bool, false, f)

// int, 默认0
#define DEFINE_GETTER_SETTER_INT(f) \
    DEFINE_GETTER_SETTER(int, 0, f)

// string, 默认空串
#define DEFINE_GETTER_SETTER_STRING_REF(f) \
    DEFINE_GETTER_SETTER(std::string, "", f)


// 各个变量
DEFINE_GETTER_SETTER_BOOL(compute_hash)
DEFINE_GETTER_SETTER_BOOL(depth_protect)
DEFINE_GETTER_SETTER_INT (max_split_files)
DEFINE_GETTER_SETTER_STRING_REF(split_files_dir)
DEFINE_GETTER_SETTER_STRING_REF(output_file)
DEFINE_GETTER_SETTER_INT (max_funcs)
DEFINE_GETTER_SETTER_INT (max_params)
DEFINE_GETTER_SETTER_INT (max_block_size)
DEFINE_GETTER_SETTER_INT (max_blk_depth)
DEFINE_GETTER_SETTER_INT (max_expr_depth)
DEFINE_GETTER_SETTER_BOOL(wrap_volatiles)
DEFINE_GETTER_SETTER_BOOL(allow_const_volatile)
DEFINE_GETTER_SETTER_BOOL(avoid_signed_overflow)
DEFINE_GETTER_SETTER_INT (max_struct_fields)
DEFINE_GETTER_SETTER_INT (max_union_fields)
DEFINE_GETTER_SETTER_INT (max_nested_struct_level)
DEFINE_GETTER_SETTER_STRING_REF(struct_output)
DEFINE_GETTER_SETTER_BOOL (fixed_struct_fields)
DEFINE_GETTER_SETTER_BOOL (expand_struct)
DEFINE_GETTER_SETTER_BOOL (use_struct)
DEFINE_GETTER_SETTER_BOOL (use_union)
DEFINE_GETTER_SETTER_INT (max_indirect_level)
DEFINE_GETTER_SETTER_INT (max_array_dimensions)
DEFINE_GETTER_SETTER_INT (max_array_length_per_dimension)
DEFINE_GETTER_SETTER_INT (max_array_length)
DEFINE_GETTER_SETTER_INT (interested_facts)
DEFINE_GETTER_SETTER_BOOL(paranoid)
DEFINE_GETTER_SETTER_BOOL(quiet)
DEFINE_GETTER_SETTER_BOOL(concise)
DEFINE_GETTER_SETTER_BOOL(nomain)
DEFINE_GETTER_SETTER_BOOL(random_based)
DEFINE_GETTER_SETTER_BOOL(dfs_exhaustive)
DEFINE_GETTER_SETTER_STRING_REF(dfs_debug_sequence)
DEFINE_GETTER_SETTER_INT (max_exhaustive_depth)
DEFINE_GETTER_SETTER_BOOL(compact_output)
DEFINE_GETTER_SETTER_BOOL(msp)
DEFINE_GETTER_SETTER_INT(func1_max_params)
DEFINE_GETTER_SETTER_BOOL(splat)
DEFINE_GETTER_SETTER_BOOL(klee)
DEFINE_GETTER_SETTER_BOOL(crest)
DEFINE_GETTER_SETTER_BOOL(ccomp)
DEFINE_GETTER_SETTER_BOOL(coverage_test)
DEFINE_GETTER_SETTER_INT(coverage_test_size)
DEFINE_GETTER_SETTER_BOOL(packed_struct)
DEFINE_GETTER_SETTER_BOOL(bitfields)
DEFINE_GETTER_SETTER_BOOL(prefix_name)
DEFINE_GETTER_SETTER_BOOL(sequence_name_prefix)
DEFINE_GETTER_SETTER_BOOL(compatible_check)
DEFINE_GETTER_SETTER_STRING_REF(partial_expand)
DEFINE_GETTER_SETTER_STRING_REF(delta_monitor)
DEFINE_GETTER_SETTER_STRING_REF(delta_output)
DEFINE_GETTER_SETTER_STRING_REF(go_delta)
DEFINE_GETTER_SETTER_STRING_REF(delta_input)
DEFINE_GETTER_SETTER_BOOL(no_delta_reduction)
DEFINE_GETTER_SETTER_BOOL(math64)
DEFINE_GETTER_SETTER_BOOL(inline_function)
DEFINE_GETTER_SETTER_BOOL(math_notmp)
DEFINE_GETTER_SETTER_BOOL(longlong)
DEFINE_GETTER_SETTER_BOOL(int8)
DEFINE_GETTER_SETTER_BOOL(uint8)
DEFINE_GETTER_SETTER_BOOL(enable_float)
DEFINE_GETTER_SETTER_BOOL(strict_float)
DEFINE_GETTER_SETTER_BOOL(pointers)
DEFINE_GETTER_SETTER_BOOL(arrays)
DEFINE_GETTER_SETTER_BOOL(strict_const_arrays)
DEFINE_GETTER_SETTER_BOOL(jumps)
DEFINE_GETTER_SETTER_BOOL(return_structs)
DEFINE_GETTER_SETTER_BOOL(arg_structs)
DEFINE_GETTER_SETTER_BOOL(return_unions)
DEFINE_GETTER_SETTER_BOOL(arg_unions)
DEFINE_GETTER_SETTER_BOOL(volatiles)
DEFINE_GETTER_SETTER_BOOL(volatile_pointers)
DEFINE_GETTER_SETTER_BOOL(const_pointers)
DEFINE_GETTER_SETTER_BOOL(global_variables)
DEFINE_GETTER_SETTER_BOOL(access_once)
DEFINE_GETTER_SETTER_BOOL(strict_volatile_rule)
DEFINE_GETTER_SETTER_BOOL(addr_taken_of_locals)
DEFINE_GETTER_SETTER_BOOL(fresh_array_ctrl_var_names)
DEFINE_GETTER_SETTER_BOOL(consts)
DEFINE_GETTER_SETTER_BOOL(builtins)
DEFINE_GETTER_SETTER_BOOL(dangling_global_ptrs)
DEFINE_GETTER_SETTER_BOOL(divs)
DEFINE_GETTER_SETTER_BOOL(muls)
DEFINE_GETTER_SETTER_BOOL(accept_argc)
DEFINE_GETTER_SETTER_BOOL(random_random)
DEFINE_GETTER_SETTER_INT(stop_by_stmt)
DEFINE_GETTER_SETTER_BOOL(step_hash_by_stmt)
DEFINE_GETTER_SETTER_BOOL(compound_assignment)
DEFINE_GETTER_SETTER_STRING_REF(dump_default_probabilities)
DEFINE_GETTER_SETTER_STRING_REF(dump_random_probabilities)
DEFINE_GETTER_SETTER_STRING_REF(probability_configuration)
DEFINE_GETTER_SETTER_BOOL(const_as_condition)
DEFINE_GETTER_SETTER_BOOL(match_exact_qualifiers)
DEFINE_GETTER_SETTER_BOOL(blind_check_global)
DEFINE_GETTER_SETTER_BOOL(no_return_dead_ptr)
DEFINE_GETTER_SETTER_BOOL(hash_value_printf)
DEFINE_GETTER_SETTER_BOOL(signed_char_index)
DEFINE_GETTER_SETTER_INT (max_array_num_in_loop)
DEFINE_GETTER_SETTER_BOOL(identify_wrappers)
DEFINE_GETTER_SETTER_BOOL(mark_mutable_const)
DEFINE_GETTER_SETTER_BOOL(force_globals_static)
DEFINE_GETTER_SETTER_BOOL(force_non_uniform_array_init)
DEFINE_GETTER_SETTER_INT(inline_function_prob)
DEFINE_GETTER_SETTER_INT(builtin_function_prob)
DEFINE_GETTER_SETTER_INT(null_pointer_dereference_prob)
DEFINE_GETTER_SETTER_INT(dead_pointer_dereference_prob)
DEFINE_GETTER_SETTER_BOOL(union_read_type_sensitive);
DEFINE_GETTER_SETTER_BOOL(pre_incr_operator);
DEFINE_GETTER_SETTER_BOOL(pre_decr_operator);
DEFINE_GETTER_SETTER_BOOL(post_incr_operator);
DEFINE_GETTER_SETTER_BOOL(post_decr_operator);
DEFINE_GETTER_SETTER_BOOL(unary_plus_operator);
DEFINE_GETTER_SETTER_BOOL(use_embedded_assigns);
DEFINE_GETTER_SETTER_BOOL(use_comma_exprs);
DEFINE_GETTER_SETTER_BOOL(take_union_field_addr);
DEFINE_GETTER_SETTER_BOOL(vol_struct_union_fields);
DEFINE_GETTER_SETTER_BOOL(const_struct_union_fields);
DEFINE_GETTER_SETTER_BOOL(lang_cpp);
DEFINE_GETTER_SETTER_BOOL(cpp11);
DEFINE_GETTER_SETTER_BOOL(fast_execution);


// 设置变量为默认值
void CGOptions::set_default_settings(void) {
    set_platform_specific_options();

}

// 在工作目录中查找平台信息文件并加载平台特定信息。
// 如果未找到，请使用Csmith正在运行的平台上的信息，并将其输出到文件中。
void CGOptions::set_platform_specific_options(void) {
    const char* int_str = "integer size = ";
	const char* ptr_str = "pointer size = ";
	ifstream conf(PLATFORM_CONFIG_FILE);
	if (conf.fail()) {
		ofstream conf(PLATFORM_CONFIG_FILE);
		conf << int_str << sizeof(int) << endl;
		conf << ptr_str << sizeof(int*) << endl;
		int_size(sizeof(int));
		pointer_size(sizeof(int*));
		conf.close();
	}
	else {
		string line;
		while(!conf.eof()) {
			getline(conf, line);
			if (line.substr(0, strlen(int_str)) == int_str) {
				string s = line.substr(strlen(int_str));
				StringUtils::chop(s);
				int_size(StringUtils::str2int(s));
			}
			if (line.substr(0, strlen(ptr_str)) == ptr_str) {
				string s = line.substr(strlen(ptr_str));
				StringUtils::chop(s);
				pointer_size(StringUtils::str2int(s));
			}
		}
		if (!int_size_) {
			cout << "please specify integer size in " << PLATFORM_CONFIG_FILE << endl;
			exit(-1);
		}
		if (!pointer_size_) {
			cout << "please specify pointer size in " << PLATFORM_CONFIG_FILE << endl;
			exit(-1);
		}
		conf.close();
	}
}