#ifndef SEQUENCE_LINE_PARSER_H
#define SEQUENCE_LINE_PARSER_H

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

template <class Container>
class SequenceLineParser {
public:
	SequenceLineParser();
	~SequenceLineParser();

	static bool parse_sequence(Container &q, const std::string &seq, const char sep_char = ',');

private:
	static int str2int(const std::string &s);
};

template <class Container>
SequenceLineParser<Container>::SequenceLineParser() {}

template <class Container>
SequenceLineParser<Container>::~SequenceLineParser() {}

// 将字符串按照特定的字符分割开，并将分割后的子串转换成int，存入q中
template <class Container>
bool SequenceLineParser<Container>::parse_sequence(Container &q, const std::string &seq, const char sep_char) {
	size_t pos = 0;
	size_t start_pos = 0;

	//while (pos < static_cast<int>(seq.size())) {
	while(true) {
		while (seq[pos] == ' ')
			pos++;

		start_pos = pos;
		pos = seq.find_first_of(sep_char, pos);
		std::string s = seq.substr(start_pos, (pos - start_pos));
		int i = SequenceLineParser::str2int(s);
		if (i < 0) return false;
		q.push_back(i);
		if (pos == string::npos) break;
		pos++;
	}
	return true;
}

// 将string转成int值
template <class Container>
int SequenceLineParser<Container>::str2int(const std::string &s)
{
	stringstream ss(s);
	int i = -1;
	ss >> i;

	return i;
}

#endif // SEQUENCE_PARSER_H