#ifndef SRC_UTIL_LOG_H_
#define SRC_UTIL_LOG_H_

#include <stdio.h>

extern FILE *LOG_file;

#define NO_OP ((void) 0)

#ifndef NO_LOG
// for when we want logging

#define LOG(fmt, args...) fprintf(LOG_file, "%s:%d ==> " fmt "\n", __FILE__, __LINE__, ##args)
#define LOG_IF(bool_exp, fmt, args...) {if(bool_exp) { LOG(fmt, ##args); }}

#else
// for when we don't want logging

#define LOG(...) NO_OP
#define LOG_IF(...) NO_OP

#endif

#endif  // SRC_UTIL_LOG_H_
