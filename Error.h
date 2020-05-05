#ifndef ERROR_H
#define ERROR_H

#include "CommonMacros.h"

#define SUCCESS 0
#define ERROR -1
#define EXCEED_MAX_DEPTH_ERROR -2
#define FILTER_ERROR -3
#define BACKTRACKING_ERROR -4
#define COMPATIBLE_CHECK_ERROR -5
#define INVALID_SIMPLE_DELTA_SEQUENCE -6

#define ERROR_RETURN() \
	if (Error::get_error() != SUCCESS) \
		return;\

#define ERROR_GUARD(rv) \
	if (Error::get_error() != SUCCESS) \
		return rv; \

#define ERROR_GUARD_AND_DEL1(rv, p) \
	if (Error::get_error() != SUCCESS) {\
		delete p; \
		return rv; \
	} \

#define ERROR_GUARD_AND_DEL2(rv, p1, p2) \
	if (Error::get_error() != SUCCESS) {\
		delete p1; \
		delete p2; \
		return rv; \
	} \

#define ERROR_GUARD_AND_DEL3(rv, p1, p2, p3) \
	if (Error::get_error() != SUCCESS) {\
		delete p1; \
		delete p2; \
		delete p3; \
		return rv; \
	} \

#define ERROR_GUARD_AND_DEL4(rv, p1, p2, p3, p4) \
	if (Error::get_error() != SUCCESS) {\
		delete p1; \
		delete p2; \
		delete p3; \
		delete p4; \
		return rv; \
	} \


#define PRT_ERROR(msg) \
	cout << "error: " << msg << ", errorno: " << Error::get_error() << std::endl; \

class Error {
public:
	static int get_error() { return Error::r_error_; }
	static void set_error(int error) { Error::r_error_ = error; }
private:
	Error();
	~Error();
	static int r_error_;

	DISALLOW_COPY_AND_ASSIGN(Error);
};

#endif // ERROR_H
