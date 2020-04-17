#ifndef SIMPLE_DELTA_SEQUENCE_H
#define SIMPLE_DELTA_SEQUENCE_H

#include <map>
#include <string>
#include <vector>
#include <ostream>
#include "Common.h"
#include "Sequence.h"

class SimpleDeltaSequence : public Sequence {
public:
	// 根据分隔符来获得一个SimpleDeltaSequence的例子
	static SimpleDeltaSequence *CreateInstance(const char sep_char);

	virtual ~SimpleDeltaSequence();
	// 根据输入文件路径，读取并分析出value-bound对，并将其存入sequence_中
	virtual void init_sequence();
	// 获取sequence_的长度
	virtual unsigned INT64 sequence_length();
	// 将v和bound构建成ValuePair，存入到seq_map_中
	virtual void add_number(int v, int bound, int k);
	// 先判断bound是否和current_pos_匹配，匹配则返回对应的value值，同时将sequence_中的值存入seq_map_中
	virtual int get_number(int bound);
	// 暂时弃用
	virtual int get_number_by_pos(int pos);
	// 清空seq_map_
	virtual void clear();
	// 将seq_map_中所有值对输出
	virtual void get_sequence(std::ostream &);
	// 获取分隔符
	virtual char get_sep_char() const { return sep_char_; }
	// 默认分隔符为','
	static const char default_sep_char;

private:
	class ValuePair;

	explicit SimpleDeltaSequence(const char sep_char);
	// 判断line是否为空
	bool empty_line(const std::string &line);
	// 将p中的value和bound打印出来
	void output_one(std::ostream &out, const ValuePair *p);

	std::map<int, ValuePair*> seq_map_;
	std::map<int, ValuePair*> sequence_;

	static SimpleDeltaSequence *impl_;

	// 分割字符默认为','
	const char sep_char_;

	unsigned INT64 current_pos_;
};

#endif // SIMPLE_DELTA_SEQUENCE_H
