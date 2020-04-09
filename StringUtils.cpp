#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "StringUtils.h"
#include "Common.h"
#include <cassert>
#include <sstream>

using namespace std;

bool StringUtils::empty_line(const std::string &line) {
    if (line.empty()) return true;
    size_t found = line.find_first_not_of("\t\n");
    return (found == string::npos);
}

bool StringUtils::is_space(const char c) {
	return ((c == ' ') || (c == '\t') || (c == '\n'));
}

void StringUtils::ignore_spaces(const string &str, size_t &pos) {
    while (is_space(str[pos])) {
        ++pos;
    }
}

std::string StringUtils::get_substring(const std::string &s, const char open_delim, const char close_delim) {
    if (s.empty()) return "";
	size_t pos = 0;
	ignore_spaces(s, pos);

	if (s[pos] != open_delim) return "";

	pos++;
	return get_substring_before(s, pos, close_delim);
}

std::string StringUtils::get_substring_before(const std::string &s, size_t &pos, const char close_delim) {
    size_t end_pos = s.find_first_of(close_delim, pos);
    if (end_pos == string::npos || end_pos == pos) return "";
	return s.substr(pos, (end_pos - pos));
}

char StringUtils::first_nonspace_char(const std::string &s) {
    if (s.empty()) return '\0';
    size_t pos = 0;
    ignore_spaces(s, pos);
    return s[pos];
}

size_t StringUtils::find_any_char(const string &s, size_t pos, const string& to_match) {
    size_t i;
	if (s.empty() || to_match.empty()) {
		return string::npos;
	}
	for (i=pos; i<s.length(); i++) {
		if (to_match.find(s[i]) != string::npos) {
			return i;
		}
	}
	return string::npos;
}

void StringUtils::split_string(const string str, vector<string> &v, const char sep_char) {
    size_t pos = 0;
	size_t start_pos = 0;
	while(true) {
		ignore_spaces(str, pos);
		start_pos = pos;
		pos = str.find_first_of(sep_char, pos);
		std::string s = str.substr(start_pos, (pos - start_pos));
		if (!s.empty()) {
			v.push_back(s);
		}
		if (pos == string::npos) {
            break;
        }
		pos++;
	}
}

void StringUtils::split_string(const string str, vector<string> &v, string sep_chars) {
    size_t pos = 0;
	size_t start_pos = 0;
	while(true) {
		ignore_spaces(str, pos);
		start_pos = pos;
		pos = find_any_char(str, pos, sep_chars);
		std::string s = str.substr(start_pos, (pos - start_pos));
		if (!s.empty()) {
			v.push_back(s);
		}
		if (pos == string::npos) {
            break;
        }
		pos++;
	}
}

void StringUtils::split_int_string(const string str, vector<int> &values, string sep_chars) {
	size_t pos = 0;
	size_t start_pos = 0;
	while(true) {
		ignore_spaces(str, pos);
		start_pos = pos;
		pos = find_any_char(str, pos, sep_chars);
		std::string s = str.substr(start_pos, (pos - start_pos));
		if (!s.empty()) {
			values.push_back(str2int(s));
		}
		if (pos == string::npos) {
            break;
        }
		pos++;
	}
}

int StringUtils::str2int(const std::string &s) {
    if (!s.empty() && s[0] == '(') {
        assert(s[s.length() - 1] == ')');
        return str2int(s.substr(1, s.length() - 2));
    }
    stringstream ss(s);
    int i = -1;
    if (s.find("0x") == 0) {
        ss >> std::hex >> i;
    } else {
        ss >> i;
    }
    return i;
}

std::string StringUtils::int2str(int i) {
    ostringstream oss;
    oss << i;
    return oss.str();
}

INT64 StringUtils::str2longlong(const std::string &s) {
    INT64 i = 0;
	size_t j;
	if (s.find("0x")==0) {
		for (j=2; j<s.length(); j++) {
			int v = 0;
			if (s[j] >= '0' && s[j] <= '9') {
				v = s[j] - '0';
			} else if (s[j] >= 'A' && s[j] <= 'F') {
				v = 10 + s[j] - 'A';
			} else {
				break;
			}
			i = i * 16 + v;
		}
	}
	else {
		stringstream ss(s);
		ss >> i;
	}
	return i;
}

std::string StringUtils::longlong2str(INT64 i) {
	ostringstream oss;
	oss << i;
	return oss.str();
}

void StringUtils::chop(string& str) {
	string s;
	size_t last;
	while (str[0] == '\t' || str[0] == ' ') {
		str = str.substr(1);
	}
	last = str.length() - 1;
	while (str[last] == '\t' || str[last] == ' ') {
		str = str.substr(0, last);
		last--;
	}
}

void StringUtils::breakup_assigns(const string& assigns, vector<string>& vars, vector<string>& values) {
	vector<string> tmp_strs;
	StringUtils::split_string(assigns, tmp_strs, ';');
	size_t i;

	string short_assigns;
	for (i=0; i<tmp_strs.size(); i++) {
		vector<string> pair;
		StringUtils::split_string(tmp_strs[i], pair, '=');
		assert(pair.size() == 2);
		StringUtils::chop(pair[0]);
		StringUtils::chop(pair[1]);
		vars.push_back(pair[0]);
		values.push_back(pair[1]);
	}
}

bool StringUtils::end_with(string s, string tail) {
	if (tail.length() < s.length()) {
		s = s.substr(s.length() - tail.length());
	}
	return s == tail;
}


StringUtils::StringUtils() {}

StringUtils::~StringUtils() {}