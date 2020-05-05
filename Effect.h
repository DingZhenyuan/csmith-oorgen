#ifndef EFFECT_H
#define EFFECT_H

///////////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <vector>

class Variable;
class Block;
class ExpressionVariable;

///////////////////////////////////////////////////////////////////////////////

class Effect
{
public:
	Effect(void);
	Effect(const Effect &e);
	~Effect(void);

	Effect &operator=(const Effect &e);

	void read_var(const Variable *v);
	void access_deref_volatile(const Variable *v, int deref_level);
	void write_var(const Variable *v);
	void write_var_set(const std::vector<const Variable *>& vars);
	void add_effect(const Effect &e, bool include_lhs_effects = false);
	void add_external_effect(const Effect &e, std::vector<const Block*> call_chain);
	void add_external_effect(const Effect &e);
	void clear(void);

	bool is_pure(void) const				{ return pure; }
	bool is_side_effect_free(void) const	{ return side_effect_free; }

	bool is_read(const Variable *v) const;
	bool is_read(std::string vname) const;
	bool is_written(const Variable *v) const;
	bool is_written(std::string vname) const;
	bool field_is_read(const Variable *v) const;
	bool field_is_written(const Variable *v) const;
	bool sibling_union_field_is_read(const Variable *v) const;
	bool sibling_union_field_is_written(const Variable *v) const;
	bool is_read_partially(const Variable* v) const;
	bool is_written_partially(const Variable* v) const;
	bool union_field_is_read(void) const;
	bool has_race_with(const Effect &e) const;
	bool is_empty(void) const;
	void consolidate(void);

	static const Effect &get_empty_effect(void)	{ return Effect::empty_effect; }

	const std::vector<const Variable *>& get_read_vars(void) const { return read_vars;}
	const std::vector<const Variable *>& get_write_vars(void) const { return write_vars;}
	const std::vector<const Variable *>& get_lhs_write_vars(void) const { return lhs_write_vars;}
	void set_lhs_write_vars(const std::vector<const Variable *>& vars)  { lhs_write_vars = vars;}

	typedef enum {
		READ, WRITE
	} Access;

	void Output(std::ostream &out) const;
	bool has_global_effect(void) const;
	void update_purity(void);

private:
	std::vector<const Variable *> read_vars;
	std::vector<const Variable *> write_vars;
	std::vector<const Variable *> lhs_write_vars;

	bool pure;
	bool side_effect_free;

	static const Effect empty_effect;
};

///////////////////////////////////////////////////////////////////////////////

#endif // EFFECT_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.
