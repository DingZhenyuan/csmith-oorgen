#ifndef DEFAULT_OUTPUT_MGR_H
#define DEFAULT_OUTPUT_MGR_H

#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include "OutputMgr.h"

using namespace std;

class DefaultOutputMgr : public OutputMgr {
public:
	static DefaultOutputMgr *CreateInstance();

	virtual ~DefaultOutputMgr();

	static bool create_output_dir(std::string dir);

	virtual void OutputHeader(int argc, char *argv[], unsigned long seed);

	virtual void Output();
	// 新的输出函数
	void OutputFunc(ofstream &out_c);

	virtual void outputln(ostream &out);

	virtual void output_comment_line(ostream &out, const std::string &comment);

	virtual void output_tab(ostream &out, int indent);

	// 设定输出
	// void setOutput(ofstream fout) {
	// 	*ofile_ = fout;
	// }

private:
	explicit DefaultOutputMgr(std::ofstream *ofile);

	DefaultOutputMgr();

	virtual std::ostream &get_main_out();

	bool is_split();

	std::ofstream* open_one_output_file(int num);

	void init();

	void OutputGlobals();

	void OutputAllHeaders();

	void RandomOutputDefs();

	void RandomOutputVarDefs();

	void RandomOutputFuncDefs();

	static DefaultOutputMgr *instance_;

	std::vector<std::ofstream* > outs;

	// 输出out
	std::ofstream *ofile_;
};

#endif // DEFAULT_OUTPUT_MGR_H
