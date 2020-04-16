#ifndef ERROR_H
#define ERROR_H

#include "CommonMacros.h"

#define SUCCESS 0
// 错误
#define ERROR -1
// 超出最大深度错误
#define EXCEED_MAX_DEPTH_ERROR -2
// 过滤器错误
#define FILTER_ERROR -3
// 回溯错误
#define BACKTRACKING_ERROR -4
// 兼容性检测错误
#define COMPATIBLE_CHECK_ERROR -5
// 无效的简单增量序列
#define INVALID_SIMPLE_DELTA_SEQUENCE -6

// 遇到错误则直接返回
#define ERROR_RETURN() \
	if (Error::get_error() != SUCCESS) \
		return;\

// 遇到错误则将rv返回
#define ERROR_GUARD(rv) \
	if (Error::get_error() != SUCCESS) \
		return rv; \

// 遇到错误则返回rv并删除p
#define ERROR_GUARD_AND_DEL1(rv, p) \
	if (Error::get_error() != SUCCESS) {\
		delete p; \
		return rv; \
	} \

// 遇到错误则返回rv，并删除p1，p2
#define ERROR_GUARD_AND_DEL2(rv, p1, p2) \
	if (Error::get_error() != SUCCESS) {\
		delete p1; \
		delete p2; \
		return rv; \
	} \

// 遇到错误则返回rv，并删除p1，p2，p3
#define ERROR_GUARD_AND_DEL3(rv, p1, p2, p3) \
	if (Error::get_error() != SUCCESS) {\
		delete p1; \
		delete p2; \
		delete p3; \
		return rv; \
	} \

// 遇到错误则返回rv，并删除p1-4
#define ERROR_GUARD_AND_DEL4(rv, p1, p2, p3, p4) \
	if (Error::get_error() != SUCCESS) {\
		delete p1; \
		delete p2; \
		delete p3; \
		delete p4; \
		return rv; \
	} \

// 打印错误信息
#define PRT_ERROR(msg) \
	cout << "error: " << msg << ", errorno: " << Error::get_error() << std::endl; \

class Error {
public:
    static int get_error() { return Error::r_error_; }
    static void set_error(int error) { Error::r_error_ = error; }
private:
    Error();
    ~Error();
    // 记录错误类型，默认为SUCCESS
    static int r_error_;

    DISALLOW_COPY_AND_ASSIGN(Error);
};

#endif // ERROR_H
