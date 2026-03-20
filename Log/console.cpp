#include "base.hpp"
#include <stdarg.h>

#define LOG_DEBUG_STR   "debug"
#define LOG_INFO_STR    "info"
#define LOG_WARNING_STR "warning"
#define LOG_ERROR_STR   "error"
#define LOG_FATAL_STR   "fatal"

#define LOG_MSG_SIZE_MAX 512

void log_init()
{
}

void log(Logger::Severity severity, const char* msg)
{
    switch(severity)
    {
    case Logger::Debug:     printf("<" LOG_DEBUG_STR   "> : %s", msg);break;
    case Logger::Info:      printf("<" LOG_INFO_STR    "> : %s", msg);break;
    case Logger::Warning:   printf("<" LOG_WARNING_STR "> : %s", msg);break;
    case Logger::Error:     printf("<" LOG_ERROR_STR   "> : %s", msg);break;
    case Logger::Fatal:     printf("<" LOG_FATAL_STR    "> : %s", msg);break;
    default:return ;
    }
}

void log_f(Logger::Severity severity, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log_v(severity, format, args);
    va_end(args);
}

void log_v(Logger::Severity severity, const char* format, va_list args)
{
    char msg[LOG_MSG_SIZE_MAX];
    vsprintf(msg, format, args);
    log(severity, msg);
}
