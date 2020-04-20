#ifndef DELTA_MONITOR_H
#define DELTA_MONITOR_H

#include <string>
#include <ostream>

class SequenceFactory;
class Sequence;

// delta种类
enum DELTA_TYPE {
	dSimpleDelta,
};

#define MAX_DELTA_TYPE ((DELTA_TYPE) (dSimpleDelta+1))

class DeltaMonitor {
public:
	// 获取一个Sequence，这里直接返回一个SimpleDeltaSequence
	static Sequence* GetSequence();
	// 获取分隔符
	static char GetSepChar();

	static void CreateRndNumInstance(const unsigned long seed);

	static bool is_running() { return is_running_; }

	static bool is_delta() { return is_delta_; }

	static bool no_delta_reduction() { return no_delta_reduction_; }
	// 初始化，检测monitor_type应不为空，并由此设定delta_type_;设定增量输出文件路径，若o_file为空则设定报错信息，
	static bool init(std::string &msg, const std::string &monitor_type, const std::string &o_file);
	// 为运行初始化，设定输入输出路径，设定delta_type，标志delta
	static bool init_for_running(std::string &msg, const std::string &monitor_type,
				const std::string &o_file, const std::string &i_file, bool no_delta);
	// 设置delta_type_，如果monitor_type为simple则设置delta_type_为dSimpleDelta，设置为运行模式，返回true，否则返回false
	static bool set_delta_type(std::string &msg, const std::string &monitor_type);
	// 若is_delta,则输出统计信息。将序列输出到输出文件路径
	static void Output(std::ostream &out);

	// 获取输入文件路径
	static const std::string &get_input();
	// 获取输出文件路径
	static const std::string &get_output();

private:
	DeltaMonitor();

	~DeltaMonitor();
	// 输出统计信息，随机点+序列长度，调用SimpleDeltaRndNumGenerator中函数
	static void OutputStatistics(std::ostream &out);
    // 类型
    static DELTA_TYPE delta_type_;
    // 输出文件路径
	static std::string output_file_;
    // 输入文件路径
	static std::string input_file_;
    // 是否在运行
	static bool is_running_;
    // 
	static bool is_delta_;
    //
	static bool no_delta_reduction_;
    // 
	static Sequence *seq_;
};

#endif
