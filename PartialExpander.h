#ifndef PARTIAL_EXPANDER_H
#define PARTIAL_EXPANDER_H

#include <string>
#include <map>
#include "Statement.h"

class PartialExpander {
public:
	static bool init_partial_expander(const std::string &options);

	static void restore_init_values();

	static bool expand_check(eStatementType t);

	static bool direct_expand_check(eStatementType t);

private:
	PartialExpander();

	~PartialExpander();

	static void init_map(map<eStatementType, bool> &m, bool value);

	static void set_stmt_expand(eStatementType t, bool value);

	static void copy_expands(std::map<eStatementType, bool> &dest, const map<eStatementType, bool> &src);

	static bool set_expand(const std::string s);

	static bool parse_options(const std::string &options, char sep_char);

	static std::map<eStatementType, bool> expands_;

	static std::map<eStatementType, bool> expands_backup_;
};

#endif
