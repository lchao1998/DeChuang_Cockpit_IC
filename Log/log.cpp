#include "log.hpp"
#include <stdarg.h>

#define __V_LOG(sev, fmt) \
va_list args; \
va_start(args, fmt); \
log_v(sev, fmt, args); \
va_end(args);

//ostream style log
Logger logDebug()     { return Logger(Logger::Debug);   }
Logger logInfo()      { return Logger(Logger::Info);    }
Logger logWarning()   { return Logger(Logger::Warning); }
Logger logError()     { return Logger(Logger::Error);   }
Logger logFatal()     { return Logger(Logger::Fatal);   }

void logDebug(const char* format, ...)   { __V_LOG(Logger::Debug, format)}
void logInfo(const char* format, ...)    { __V_LOG(Logger::Info, format)}
void logWarning(const char* format, ...) { __V_LOG(Logger::Warning, format)}
void logError(const char* format, ...)   { __V_LOG(Logger::Error, format)}
void logFatal(const char* format, ...)   { __V_LOG(Logger::Fatal, format)}
