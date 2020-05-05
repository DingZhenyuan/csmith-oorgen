#ifndef DEFAULT_PROGRAM_GENERATOR_H
#define DEFAULT_PROGRAM_GENERATOR_H

#include "AbsProgramGenerator.h"

class OutputMgr;

class DefaultProgramGenerator : public AbsProgramGenerator {
public:
	DefaultProgramGenerator(int argc, char *argv[], unsigned long seed);

	virtual ~DefaultProgramGenerator();

	virtual OutputMgr* getOutputMgr() { return output_mgr_; }

	virtual void goGenerator();

	virtual void initialize();

	virtual std::string get_count_prefix(const std::string &name);

private:
	int argc_;

	char **argv_;

	unsigned long seed_;

	OutputMgr *output_mgr_;
};

#endif
