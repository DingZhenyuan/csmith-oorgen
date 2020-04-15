#ifndef SEQUENCE_FACTORY_H
#define SEQUENCE_FACTORY_H

#include <set>

class Sequence;

class SequenceFactory {
public:
	// 根据DeltaMonitor是否运行判断，获取一个Sequence,并将其加入到seqs_中，并判定current_sep_char_
	static Sequence *make_sequence();
	// 清空seqs_
	static void destroy_sequences();
	// 获取current_sep_char_
	static char current_sep_char() { return current_sep_char_; }

private:
	static std::set<Sequence*> seqs_;

	// 当前分隔符，默认为下划线'_'
	static char current_sep_char_;
};

#endif // SEQUENCE_FACTORY_H