#ifndef OUTPUT_MGR_H
#define OUTPUT_MGR_H

#include <ostream>
#include <fstream>
#include <vector>
#include <string>

#include "ClassType.h"

using namespace std;

class Variable;

#define TAB "    "    // to beautify output: 1 tab is 4 spaces

class OutputMgr {
public:
	OutputMgr();

	virtual ~OutputMgr() = 0;

	static void OutputHashFuncDef(std::ostream &out);

	static void OutputHashFuncDecl(std::ostream &out);

	static void OutputHashFuncInvocation(std::ostream &out, int indent);

	static void OutputStepHashFuncInvocation(std::ostream &out, int indent, int stmt_id);

	static void OutputStepHashFuncDecl(std::ostream &out);

	static void OutputStepHashFuncDef(std::ostream &out);

	static void really_outputln(std::ostream &out);

	static void set_curr_func(const std::string &fname);

	virtual void OutputHeader(int argc, char *argv[], unsigned long seed) = 0;

	// 输出头，按类文件
	void OutputHeaderClass(int argc, char *argv[], unsigned long seed, ofstream &out_c);

	// 输出class定义
	void OutputClass(ClassType classType, ofstream &out_c);

	virtual void OutputStructUnions(ostream& /* out */) {};

	virtual void Output() = 0;
	// 新的输出函数
	virtual void OutputFunc(ofstream &out_c);
	// 新的输出函数
	virtual void OutputFunc(int funcIndex, int funcNumPerClass, ofstream &out_c, bool outVariable);

	virtual void outputln(ostream &out) {out << std::endl;}

	virtual void output_comment_line(ostream &out, const std::string &comment);

	virtual void output_tab(ostream &out, int indent);

	void OutputPtrResets(ostream &out, const vector<const Variable*>& ptrs);

	static const char *hash_func_name;

	static const char *step_hash_func_name;

	static vector<std::string> monitored_funcs_;

protected:
	virtual std::ostream &get_main_out() = 0;

	static void output_tab_(ostream &out, int indent);

	void OutputTail(std::ostream &out);

	void OutputMain(std::ostream &out);

private:

	static bool is_monitored_func(void);

	static std::string curr_func_;

};

#endif // OUTPUT_MGR_H
