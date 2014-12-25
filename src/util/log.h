#ifndef SRC_UTIL_LOG_H_
#define SRC_UTIL_LOG_H_

#include <stdio.h>

extern FILE *LOG_file;
extern int LOG_level;

#define NO_OP ((void) 0)

#ifndef NO_LOG
// for when we want logging

#define LOG(fmt, args...) fprintf(LOG_file, "%s:%d ==> " fmt "\n", __FILE__, __LINE__, ##args)

#define LOG_(type, fmt, args...) \
{fprintf(LOG_file, type ": %s:%d ==> " fmt "\n", __FILE__, __LINE__, ##args); }

#define LOG_E(fmt, args...) {if (4 >= LOG_level) { LOG_("ERROR", fmt, ##args); }}
#define LOG_W(fmt, args...) {if (3 >= LOG_level) { LOG_("WARNING", fmt, ##args); }}
#define LOG_I(fmt, args...) {if (2 >= LOG_level) { LOG_("INFO", fmt, ##args); }}
#define LOG_D(fmt, args...) {if (1 >= LOG_level) { LOG_("DEBUG", fmt, ##args); }}
#define LOG_T(fmt, args...) {if (0 >= LOG_level) { LOG_("TRACE", fmt, ##args); }}

#define LOG_IF(bool_exp, fmt, args...) {if(bool_exp) { LOG(fmt, ##args); }}

#define LOG_IF_E(bool_exp, fmt, args...)\
{if ((4 >= LOG_level) && (bool_exp)) { LOG_("ERROR", fmt, ##args); }}
#define LOG_IF_W(bool_exp, fmt, args...)\
{if ((3 >= LOG_level) && (bool_exp)) { LOG_("WARNING", fmt, ##args); }}
#define LOG_IF_I(bool_exp, fmt, args...)\
{if ((2 >= LOG_level) && (bool_exp)) { LOG_("INFO", fmt, ##args); }}
#define LOG_IF_D(bool_exp, fmt, args...)\
{if ((1 >= LOG_level) && (bool_exp)) { LOG_("DEBUG", fmt, ##args); }}
#define LOG_IF_T(bool_exp, fmt, args...)\
{if ((0 >= LOG_level) && (bool_exp)) { LOG_("TRACE", fmt, ##args); }}

#else
// for when we don't want logging

#define LOG(...) NO_OP

#define LOG_E(fmt, args...) NO_OP
#define LOG_W(fmt, args...) NO_OP
#define LOG_I(fmt, args...) NO_OP
#define LOG_D(fmt, args...) NO_OP
#define LOG_T(fmt, args...) NO_OP

#define LOG_IF(...) NO_OP

#define LOG_IF_E(bool_exp, fmt, args...)
#define LOG_IF_W(bool_exp, fmt, args...)
#define LOG_IF_I(bool_exp, fmt, args...)
#define LOG_IF_D(bool_exp, fmt, args...)
#define LOG_IF_T(bool_exp, fmt, args...)


#endif

#endif  // SRC_UTIL_LOG_H_
