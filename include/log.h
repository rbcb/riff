/* default log format and colour from the beautiful rci/log.c */

/* @TODO multithread locking */
/* @TODO const the global writer's param */
/* @TODO cleaning file paths */

#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <time.h>

#include "os.h"

/* all the data generated by a log, to be passed to a writer */
typedef struct {
	const char *message, *func, *file, *ftime;
	unsigned line, id, level;
	struct tm* tinfo;
} log_msg_t;

/* a function that describes the transmission of log_msg_t */
typedef void (*log_writer_f)(log_msg_t* msg);

/* these should be identical to syslog's defines... */
enum {
	LOG_LEVEL_DEBUG, LOG_LEVEL_INFO, LOG_LEVEL_NOTICE,
	LOG_LEVEL_WARN, LOG_LEVEL_ERROR, LOG_LEVEL_PANIC
};

/* do nothing in log_writer_coloured/log_writer_stdout/log_writer_stderr */
void log_set_silent(void);

/* actually implement log_writer_coloured/log_writer_stdout/log_writer_stderr */
void log_set_noisy(void);

/* set the log level, any logs with a level lower than this will not show */
void log_set_level(unsigned l);

/* set the file written to by some writers */
void log_set_file(FILE* fp);

/* get the file written to by some writers */
FILE* log_get_file(void);

/* get the log level */
unsigned log_get_level(void);

/* how many logs have been made? */
unsigned log_get_count(void);

/* get the current writer */
void log_get_writer(log_writer_f* writer);

/* set the current writer */
void log_set_writer(log_writer_f writer);

/* write to stderr with ANSI colour codes if supported */
void log_writer_coloured(log_msg_t* msg);

/* write to stderr */
void log_writer_stderr(log_msg_t* msg);

/* write to stdout */
void log_writer_stdout(log_msg_t* msg);

/* write to a file */
void log_writer_file(log_msg_t* msg);

/* write to a syslog if it supported */
void log_writer_syslog(log_msg_t* msg);

/* write to stderr in ANSI colour and to a FILE* if provided */
void log_writer_colourful_and_file(log_msg_t* msg);

/* generate a log, consider using log_log instead */
void log_wrapper(
		const char* function,
		const char* file,
		unsigned line,
		unsigned level,
		const char* fmt, ...);

#ifndef LOG_NOLOG
#	define log_log(level, fmt, args...) log_wrapper(__func__, __FILE__, __LINE__, level, fmt, ##args)
#else
#	define log_log(level, fmt, args...) /* */
#endif

#define log_debug(fmt, args...) log_log(LOG_LEVEL_DEBUG, fmt, ##args)
#define log_info(fmt, args...) log_log(LOG_LEVEL_INFO, fmt, ##args)
#define log_notice(fmt, args...) log_log(LOG_LEVEL_NOTICE, fmt, ##args)
#define log_warn(fmt, args...) log_log(LOG_LEVEL_WARN, fmt, ##args)
#define log_error(fmt, args...) log_log(LOG_LEVEL_ERROR, fmt, ##args)
#define log_panic(fmt, args...) log_log(LOG_LEVEL_PANIC, fmt, ##args)

#define log_debug_if(expr, fmt, args...) do { if(expr) { log_debug(fmt,##args); } } while(0)
#define log_info_if(expr, fmt, args...) do { if(expr) { log_info(fmt,##args); } } while(0)
#define log_notice_if(expr, fmt, args...) do { if(expr) { log_notice(fmt,##args); } } while(0)
#define log_warn_if(expr, fmt, args...) do { if(expr) { log_warn(fmt,##args); } } while(0)
#define log_error_if(expr, fmt, args...) do { if(expr) { log_error(fmt,##args); } } while(0)
#define log_panic_if(expr, fmt, args...) do { if(expr) { log_panic(fmt,##args); } } while(0)

#if !defined(LOG_TIME_FMT) || !defined(LOG_TIME_FMTL)
#	define LOG_TIME_FMT "%H:%M:%S"
#	define LOG_TIME_FMTL 8
#endif

#if defined(OS_LINUX) || defined(LOG_USE_SYSLOG)
#	define LOG_USE_SYSLOG 1
#else
#	define LOG_USE_SYSLOG 0
#endif

#if !defined(LOG_DEFAULT_WRITER)
#	if !defined(LOG_SILENT)
#		define LOG_DEFAULT_WRITER log_writer_colourful_and_file
#	else
#		define LOG_DEFAULT_WRITER log_writer_file
#	endif
#endif

#endif
