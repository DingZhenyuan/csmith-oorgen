#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "OutputMgr.h"

#include <cassert>
#include <sstream>
#include <fstream>
#include "Common.h"
#include "CGOptions.h"
#include "platform.h"
#include "Bookkeeper.h"
#include "FunctionInvocation.h"
#include "Function.h"
#include "VariableSelector.h"
#include "CGContext.h"
#include "ExtensionMgr.h"
#include "Constant.h"
#include "ArrayVariable.h"
#include "git_version.h"
#include "random.h"
#include "util.h"
#include "DeltaMonitor.h"

#include "ModelReader.h"

const char *OutputMgr::hash_func_name = "csmith_compute_hash";

const char *OutputMgr::step_hash_func_name = "step_hash";

static const char runtime_include[] = "\
#include \"csmith.h\"\n\
";

static const char volatile_include[] = "\
/* To use wrapper functions, compile this program with -DWRAP_VOLATILES=1. */\n\
#include \"volatile_runtime.h\"\n\
";

static const char access_once_macro[] = "\
#ifndef ACCESS_ONCE\n\
#define ACCESS_ONCE(v) (*(volatile typeof(v) *)&(v))\n\
#endif\n\
";

using namespace std;

vector<string> OutputMgr::monitored_funcs_;

std::string OutputMgr::curr_func_ = "";

void
OutputMgr::set_curr_func(const std::string &fname)
{
	OutputMgr::curr_func_ = fname;
	// cout << fname << endl;
}

bool
OutputMgr::is_monitored_func(void)
{
	if (OutputMgr::monitored_funcs_.empty())
		return true;
	std::vector<string>::iterator result =
		find(monitored_funcs_.begin(), monitored_funcs_.end(), curr_func_);
	return (result != monitored_funcs_.end());
}

OutputMgr::OutputMgr()
{

}

OutputMgr::~OutputMgr()
{

}

void
OutputMgr::OutputMain(std::ostream &out)
{
	CGContext cg_context(GetFirstFunction() /* BOGUS -- not in first func. */,
						 Effect::get_empty_effect(),
						 0);

	FunctionInvocation *invoke = NULL;
	invoke = ExtensionMgr::MakeFuncInvocation(GetFirstFunction(), cg_context);
	out << endl << endl;
	output_comment_line(out, "----------------------------------------");

	ExtensionMgr::OutputInit(out);

	// output initializers for global array variables
	OutputArrayInitializers(*VariableSelector::GetGlobalVariables(), out, 1);

	if (CGOptions::blind_check_global()) {
		ExtensionMgr::OutputFirstFunInvocation(out, invoke);
		std::vector<Variable *>& vars = *VariableSelector::GetGlobalVariables();
		for (size_t i=0; i<vars.size(); i++) {
			vars[i]->output_value_dump(out, "checksum ", 1);
		}
	}
	else {
		// set up a global variable that controls if we print the hash value after computing it for each global
		out << "    int print_hash_value = 0;" << endl;
		if (CGOptions::accept_argc()) {
			out << "    if (argc == 2 && strcmp(argv[1], \"1\") == 0) print_hash_value = 1;" << endl;
		}

		out << "    platform_main_begin();" << endl;
		if (CGOptions::compute_hash()) {
			out << "    crc32_gentab();" << endl;
		}

		ExtensionMgr::OutputFirstFunInvocation(out, invoke);

	#if 0
		out << "    ";
		invoke->Output(out);
		out << ";" << endl;
	#endif
		// resetting all global dangling pointer to null per Rohit's request
		if (!CGOptions::dangling_global_ptrs()) {
			OutputPtrResets(out, GetFirstFunction()->dead_globals);
		}

		if (CGOptions::step_hash_by_stmt())
			OutputMgr::OutputHashFuncInvocation(out, 1);
		else
			HashGlobalVariables(out);
		if (CGOptions::compute_hash()) {
			out << "    platform_main_end(crc32_context ^ 0xFFFFFFFFUL, print_hash_value);" << endl;
		} else {
			out << "    platform_main_end(0,0);" << endl;
		}
	}
	ExtensionMgr::OutputTail(out);
	out << "}" << endl;
	delete invoke;
}

void
OutputMgr::OutputHashFuncInvocation(std::ostream &out, int indent)
{
	OutputMgr::output_tab_(out, indent);
	out << OutputMgr::hash_func_name << "();" << std::endl;
}

void
OutputMgr::OutputStepHashFuncInvocation(std::ostream &out, int indent, int stmt_id)
{
	if (is_monitored_func()) {
		OutputMgr::output_tab_(out, indent);
		out << OutputMgr::step_hash_func_name << "(" << stmt_id << ");" << std::endl;
	}
}

void
OutputMgr::OutputStepHashFuncDef(std::ostream &out)
{
	out << std::endl;
	out << "void " << OutputMgr::step_hash_func_name << "(int stmt_id)" << std::endl;
	out << "{" << std::endl;

	int indent = 1;
	OutputMgr::output_tab_(out, indent);
	out << "int i = 0;" << std::endl;
	OutputMgr::OutputHashFuncInvocation(out, indent);
	OutputMgr::output_tab_(out, indent);
	out << "printf(\"before stmt(%d): ";
	out << "checksum = %X\\n\", stmt_id, crc32_context ^ 0xFFFFFFFFUL);" << std::endl;

	OutputMgr::output_tab_(out, indent);
	out << "crc32_context = 0xFFFFFFFFUL; " << std::endl;

	OutputMgr::output_tab_(out, indent);
	out << "for (i = 0; i < 256; i++) { " << std::endl;
	OutputMgr::output_tab_(out, indent+1);
	out << "crc32_tab[i] = 0;" << std::endl;
	OutputMgr::output_tab_(out, indent);
	out << "}" << std::endl;
	OutputMgr::output_tab_(out, indent);
	out << "crc32_gentab();" << endl;
	out << "}" << std::endl;
}

void
OutputMgr::OutputHashFuncDecl(std::ostream &out)
{
	out << "void " << OutputMgr::hash_func_name << "(void);";
	out << std::endl << std::endl;
}

void
OutputMgr::OutputStepHashFuncDecl(std::ostream &out)
{
	out << "void " << OutputMgr::step_hash_func_name << "(int stmt_id);";
	out << std::endl << std::endl;
}

void
OutputMgr::OutputHashFuncDef(std::ostream &out)
{
	out << "void " << OutputMgr::hash_func_name << "(void)" << std::endl;
	out << "{" << std::endl;

	size_t dimen = Variable::GetMaxArrayDimension(*VariableSelector::GetGlobalVariables());
	if (dimen) {
		vector <const Variable*> &ctrl_vars = Variable::get_new_ctrl_vars();
		OutputArrayCtrlVars(ctrl_vars, out, dimen, 1);
	}
	HashGlobalVariables(out);
	out << "}" << std::endl;
}

void
OutputMgr::OutputTail(std::ostream &out)
{
	if (!CGOptions::concise()) {
		out << endl << "/************************ statistics *************************" << endl;
		Bookkeeper::output_statistics(out);
		out << "********************* end of statistics **********************/" << endl;
		out << endl;
	}
}


// 按类输出头文件
void OutputMgr::OutputHeaderClass(int argc, char *argv[], unsigned long seed, ofstream &out_c) {
	if (CGOptions::concise()) {
		// concise模式
		out_c << "// Options:  ";
		if (argc <= 1) {
			out_c << " (none)";
		} else {
			for (int i = 1; i < argc; ++i) {
				out_c << " " << argv[i];
			}
		}
		out_c << endl;
	} else {
		// 一般模式
		out_c << "/*" << endl;
		out_c << " * This is a RANDOMLY GENERATED PROGRAM." << endl;
		out_c << " *" << endl;
		out_c << " * Options:  ";
		if (argc <= 1) {
			out_c << " (none)";
		} else {
			for (int i = 1; i < argc; ++i) {
				out_c << " " << argv[i];
			}
		}
		out_c << endl;
		out_c << " * Seed:      " << seed << endl;
		out_c << " */" << endl;
		out_c << endl;
	}
	
	if (!CGOptions::longlong()) {
		// 不允许long long则标注
		out_c << endl;
		out_c << "#define NO_LONGLONG" << std::endl;
		out_c << endl;
	}

	if (CGOptions::enable_float()) {
		// 允许float类型
		out_c << "#include <float.h>\n";
		out_c << "#include <math.h>\n";
	}

	// 加入各class文件的头

	ExtensionMgr::OutputHeader(out_c);

	out_c << runtime_include << endl;

 	if (!CGOptions::compute_hash()) {
		if (CGOptions::allow_int64())
			out_c << "volatile uint64_t " << Variable::sink_var_name << " = 0;" << endl;
		else
			out_c << "volatile uint32_t " << Variable::sink_var_name << " = 0;" << endl;
	}
	out_c << endl;

	out_c << "static long __undefined;" << endl;
	out_c << endl;

	// 深度保护
	if (CGOptions::depth_protect()) {
		out_c << "#define MAX_DEPTH (5)" << endl;
		// Make depth signed, to cover our tails.
		out_c << "int32_t DEPTH = 0;" << endl;
		out_c << endl;
	}

	// out << platform_include << endl;
	if (CGOptions::wrap_volatiles()) {
		out_c << volatile_include << endl;
	}

	if (CGOptions::access_once()) {
		out_c << access_once_macro << endl;
	}

	if (CGOptions::step_hash_by_stmt()) {
		OutputMgr::OutputHashFuncDecl(out_c);
		OutputMgr::OutputStepHashFuncDecl(out_c);
	}
}

// 输出class定义
void OutputMgr::OutputClass(ClassType classType, ofstream &out_c) {
	// 输出类声明
	if (classType.getParent() == "" && classType.getAbstract() == false) {
		out_c << "class " << classType.getName() << endl;
		out_c << "{" << endl;
	} else if(classType.getAbstract()) {
		out_c << "virtual class " << classType.getName() << endl;
		out_c << "{" << endl;
	} else if (classType.getParent() != "") {
		out_c << "class " << classType.getName() << " : public " << classType.getParent() << endl;
		out_c << "{" << endl;
	} else {
		cout << "error: virtual & parent!" << endl;
	}

	out_c << "private:" << endl;

	// 输出属性声明
	vector<SimpleAttribute> simpleAttributes = classType.getAttributes();
	for (int i = 0; i < simpleAttributes.size(); i++) {
		if (simpleAttributes[i].value == "") {
            out_c << "    " << ModelReader::SimpleTypeToStr(simpleAttributes[i].type) << " " << simpleAttributes[i].name << ";" << endl;
        } else {
            out_c << "    " << ModelReader::SimpleTypeToStr(simpleAttributes[i].type) << " " << simpleAttributes[i].name << " = " << simpleAttributes[i].value << ";" << endl;
        }
	}

	out_c << "\npublic:" << endl;

	// 类定义结束
	// out_c << "};" << endl;
}


void OutputMgr::OutputHeader(int argc, char *argv[], unsigned long seed) {
	std::ostream &out = get_main_out();
	if (CGOptions::concise()) {
		out << "// Options:  ";
		if (argc <= 1) {
			out << " (none)";
		} else {
			for (int i = 1; i < argc; ++i) {
				out << " " << argv[i];
			}
		}
		out << endl;
	}
	else {
		out << "/*" << endl;
		out << " * This is a RANDOMLY GENERATED PROGRAM." << endl;
		out << " *" << endl;
		// out << " * Generator: " << PACKAGE_STRING << endl;
		// out << " * Git version: " << git_version << endl;
		out << " * Options:  ";
		if (argc <= 1) {
			out << " (none)";
		} else {
			for (int i = 1; i < argc; ++i) {
				out << " " << argv[i];
			}
		}
		out << endl;
		out << " * Seed:      " << seed << endl;
		out << " */" << endl;
		out << endl;
	}

	if (!CGOptions::longlong()) {
		out << endl;
		out << "#define NO_LONGLONG" << std::endl;
		out << endl;
	}
	if (CGOptions::enable_float()) {
		out << "#include <float.h>\n";
		out << "#include <math.h>\n";
	}

	ExtensionMgr::OutputHeader(out);

	out << runtime_include << endl;

 	if (!CGOptions::compute_hash()) {
		if (CGOptions::allow_int64())
			out << "volatile uint64_t " << Variable::sink_var_name << " = 0;" << endl;
		else
			out << "volatile uint32_t " << Variable::sink_var_name << " = 0;" << endl;
	}
	out << endl;

	out << "static long __undefined;" << endl;
	out << endl;

	if (CGOptions::depth_protect()) {
		out << "#define MAX_DEPTH (5)" << endl;
		// Make depth signed, to cover our tails.
		out << "int32_t DEPTH = 0;" << endl;
		out << endl;
	}

	// out << platform_include << endl;
	if (CGOptions::wrap_volatiles()) {
		out << volatile_include << endl;
	}

	if (CGOptions::access_once()) {
		out << access_once_macro << endl;
	}

	if (CGOptions::step_hash_by_stmt()) {
		OutputMgr::OutputHashFuncDecl(out);
		OutputMgr::OutputStepHashFuncDecl(out);
	}
}

void
OutputMgr::output_comment_line(ostream &out, const std::string &comment)
{
	if (CGOptions::quiet() || CGOptions::concise()) {
		outputln(out);
	}
	else {
		out << "/* " << comment << " */";
		outputln(out);
	}
}

/*
 * resetting pointers to null by outputing "p = 0;"
 */
void
OutputMgr::OutputPtrResets(ostream &out, const vector<const Variable*>& ptrs)
{
	size_t i;
	for (i=0; i<ptrs.size(); i++) {
		const Variable* v = ptrs[i];
		if (v->isArray) {
			const ArrayVariable* av = (const ArrayVariable*)v;
			Constant zero(get_int_type(), "0");
			vector<const Variable *> &ctrl_vars = Variable::get_last_ctrl_vars();
			av->output_init(out, &zero, ctrl_vars, 1);
		}
		else {
			output_tab(out, 1);
			v->Output(out);
			out << " = 0;";
			outputln(out);
		}
	}
}

void
OutputMgr::output_tab_(ostream &out, int indent)
{
	while (indent--) {
		out << TAB;
	}
}

void
OutputMgr::output_tab(ostream &out, int indent)
{
	OutputMgr::output_tab_(out, indent);
}

// void
// OutputMgr::really_outputln(ostream &out)
// {
// 	out << std::endl;
// }

void
OutputMgr::really_outputln(ostream &out)
{
	// out << std::endl;
	// 格式调整
	out << "\n    ";
}

// 新的Output
void OutputMgr::OutputFunc(ofstream &out_c) {
	// if (DeltaMonitor::is_running() && (Error::get_error() != SUCCESS)) {
	// 	out_c << "Delta reduction error!\n";
	// }
	OutputStructUnionDeclarationsClass(out_c);
	OutputGlobalVariables(out_c);
	OutputForwardDeclarationsClass(out_c);
	OutputFunctionsClass(out_c);

	// if (CGOptions::step_hash_by_stmt()) {
	// 	OutputMgr::OutputHashFuncDef(out);
	// 	OutputMgr::OutputStepHashFuncDef(out);
	// }

	// 输出Main函数
	
	// if (!CGOptions::nomain())
	// 	OutputMain(out);
	// OutputTail(out);
	// DeltaMonitor::Output(out);
}

// 新的Output
void OutputMgr::OutputFunc(int funcIndex, int funcNumPerClass, ofstream &out_c, bool outVariable, bool outMain) {
	// if (DeltaMonitor::is_running() && (Error::get_error() != SUCCESS)) {
	// 	out_c << "Delta reduction error!\n";
	// }
	
	if (outMain) {
		// 输出Main函数
		if (!CGOptions::nomain())
			OutputMain(out_c);
		OutputTail(out_c);
		DeltaMonitor::Output(out_c);
	}
	else {
		if (outVariable) {
			OutputStructUnionDeclarationsClass(out_c);
			OutputGlobalVariablesClass(funcIndex, funcNumPerClass, out_c);
		}
		// OutputForwardDeclarationsClass(out_c);
		OutputFunctionsClass(funcIndex, funcNumPerClass, out_c);
		out_c << "};" << endl;
	}


	// if (CGOptions::step_hash_by_stmt()) {
	// 	OutputMgr::OutputHashFuncDef(out);
	// 	OutputMgr::OutputStepHashFuncDef(out);
	// }	
	
}

//////////////////////////////////////////////////////////////////
