#ifndef ABS_PROGRAM_GENERATOR_H
#define ABS_PROGRAM_GENERATOR_H

#include <string>

class OutputMgr;

class AbsProgramGenerator {
public:
	static AbsProgramGenerator* CreateInstance(int argc, char *argv[], unsigned long seed);

	static AbsProgramGenerator* GetInstance();

	AbsProgramGenerator();

	virtual ~AbsProgramGenerator();

	static OutputMgr* GetOutputMgr();

	virtual void goGenerator() = 0;

	virtual OutputMgr* getOutputMgr() = 0;

	virtual std::string get_count_prefix(const std::string &name) = 0;

protected:
	virtual void initialize() = 0;

private:
	static AbsProgramGenerator *current_generator_;

	static OutputMgr *getmgr(AbsProgramGenerator *gen);
};

#endif
