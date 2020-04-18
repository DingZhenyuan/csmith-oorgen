#ifndef ENUMERATOR_H
#define ENUMERATOR_H

#include <map>
#include <cassert>

using namespace std;

template <class Name>
class Enumerator {
public:
	Enumerator();
	~Enumerator();

	void add_elem(Name name, int bound);
	void add_bool_elem(Name name, int value);
	void add_bool_elem_of_bool(Name name, bool value);

	int get_elem(Name name);

	Enumerator *next();
	Enumerator *end() { return NULL; }
	Enumerator *begin();

	bool is_changed(Name name);

private:

	class EnumObject {
	public:
		EnumObject(int bound, bool is_bool, bool bool_value)
			: bound_(bound),
		  	  current_value_(0),
			  is_bool_(is_bool),
			  bool_value_(bool_value),
			  changed_(false)
		{
			assert(bound_ > 0);
		}

		~EnumObject() { }

		int bound() { return bound_; }
		bool bool_value() { return bool_value_; }
		int get_current_value() { return current_value_; }
		bool is_bool() { return is_bool_; }
		bool next() {
			current_value_++;
			if (current_value_ < bound_) {
				changed_ = true;
				return true;
			} else {
				current_value_--;
				return false;
			}
		}

		bool good_value() {
			return (current_value_ < bound_);
		}
        
		void clear_value() { current_value_ = 0; }
		void reset_changed() { changed_ = 0; }
		bool is_changed() { return changed_; }

	private:
		const int bound_;
		int current_value_;
		const bool is_bool_;
		const bool bool_value_;
		bool changed_;
	};

	void reset_all_changed();
	void reset_pos();
	void reset_after_backward_pos();
	bool roll_back_current_pos();
	std::map<Name, EnumObject*> objs_;
	typename std::map<Name, EnumObject*>::iterator forward_pos_;
	typename std::map<Name, EnumObject*>::iterator backward_pos_;
};

#endif // ENUMERATOR_H