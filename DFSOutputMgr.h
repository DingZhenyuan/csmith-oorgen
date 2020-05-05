#ifndef DFS_OUTPUT_MGR_H
#define DFS_OUTPUT_MGR_H

#include "OutputMgr.h"

#define DEFAULT_STRUCT_OUTPUT "csmith_structs.h"

class DFSOutputMgr : public OutputMgr {
public:
	static DFSOutputMgr *CreateInstance();

	virtual ~DFSOutputMgr();

	virtual void OutputHeader(int argc, char *argv[], unsigned long seed);

	virtual void OutputStructUnions(ostream& /*out*/);

	virtual void Output();

	virtual void outputln(ostream &out);

	virtual void output_comment_line(ostream &out, const std::string &comment);

	virtual void output_tab(ostream &out, int indent);

private:
	DFSOutputMgr();

	virtual std::ostream &get_main_out();

	static DFSOutputMgr *instance_;

	std::string struct_output_;
};

#endif // DFS_OUTPUT_MGR_H
