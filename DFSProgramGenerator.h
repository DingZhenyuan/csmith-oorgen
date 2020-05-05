#ifndef DFS_PROGRAM_GENERATOR_H
#define DFS_PROGRAM_GENERATOR_H

#include "AbsProgramGenerator.h"
#include "Common.h"

class OutputMgr;

class DFSProgramGenerator : public AbsProgramGenerator {
public:
	DFSProgramGenerator(int argc, char *argv[], unsigned long seed);

	virtual ~DFSProgramGenerator();

	virtual OutputMgr* getOutputMgr() { return output_mgr_; }

	virtual void goGenerator();

	virtual void initialize();

	virtual std::string get_count_prefix(const std::string &name);

private:
	int argc_;

	char **argv_;

	unsigned long seed_;

	INT64 good_count_;

	OutputMgr *output_mgr_;
};

#endif
