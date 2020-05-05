#ifndef DELTA_MONITOR_H
#define DELTA_MONITOR_H

#include <string>
#include <ostream>

class SequenceFactory;
class Sequence;

enum DELTA_TYPE {
	dSimpleDelta,
};

#define MAX_DELTA_TYPE ((DELTA_TYPE) (dSimpleDelta+1))

class DeltaMonitor {
public:
	static Sequence *GetSequence();

	static char GetSepChar();

	static void CreateRndNumInstance(const unsigned long seed);

	static bool is_running() { return is_running_; }

	static bool is_delta() { return is_delta_; }

	static bool no_delta_reduction() { return no_delta_reduction_; }

	static bool init(std::string &msg, const std::string &monitor_type, const std::string &o_file);

	static bool init_for_running(std::string &msg, const std::string &monitor_type,
				const std::string &o_file, const std::string &i_file, bool no_delta);

	static bool set_delta_type(std::string &msg, const std::string &monitor_type);

	static void Output(std::ostream &out);

	static const std::string &get_input();

	static const std::string &get_output();

private:
	DeltaMonitor();

	~DeltaMonitor();

	static void OutputStatistics(std::ostream &out);

	static DELTA_TYPE delta_type_;

	static std::string output_file_;

	static std::string input_file_;

	static bool is_running_;

	static bool is_delta_;

	static bool no_delta_reduction_;

	static Sequence *seq_;
};

#endif
