#ifndef COMMON_MACROS_H
#define COMMON_MACROS_H

// Macro for disallowing copy and assign constructors
#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
  ClassName(const ClassName&);               \
  void operator=(const ClassName&)

#endif // COMMON_MACROS_H
