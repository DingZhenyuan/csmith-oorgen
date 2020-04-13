#ifndef COMMON_MACROS_H
#define COMMON_MACROS_H

// 禁止复制和分配构造函数
#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
  ClassName(const ClassName&);               \
  void operator=(const ClassName&)

#endif // COMMON_MACROS_H
