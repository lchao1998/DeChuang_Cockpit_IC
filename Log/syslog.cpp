#include "base.hpp"
#include <syslog.h>
#include <stdarg.h>
#include <map>

#define LOG_DEFAULT_IDENT               "unknow"
#define LOG_DEFAULT_FACILITY            LOG_LOCAL0
#define LOG_DEFAULT_OPTIONS             (LOG_PID)

#ifndef LOG_MSG_SIZE_MAX
#define LOG_MSG_SIZE_MAX                256
#endif

#define LOG_DEBUG_STR   "debug"
#define LOG_INFO_STR    "info"
#define LOG_WARNING_STR "warning"
#define LOG_ERROR_STR   "error"
#define LOG_CRIT_STR    "crit"

static const std::map<std::string, int> logFacilityMapping =
{
    {"auth",        LOG_AUTH},
    {"autopriv",    LOG_AUTHPRIV},
    {"cron",        LOG_CRON},
    {"daemon",      LOG_DAEMON},
    {"ftp",         LOG_FTP},
    {"kern",        LOG_KERN},
    {"local0",      LOG_LOCAL0},
    {"local1",      LOG_LOCAL1},
    {"local2",      LOG_LOCAL2},
    {"local3",      LOG_LOCAL3},
    {"local4",      LOG_LOCAL4},
    {"local5",      LOG_LOCAL5},
    {"local6",      LOG_LOCAL6},
    {"local7",      LOG_LOCAL7},
    {"lpr",         LOG_LPR},
    {"mail",        LOG_MAIL},
    {"news",        LOG_NEWS},
    {"syslog",      LOG_SYSLOG},
    {"user",        LOG_USER},
    {"uucp",        LOG_UUCP},
};

const char* getLogIdent()
{
    char* ev = getenv("LOG_SYSLOG_IDENT");
    if(ev)
        return ev;
    return LOG_DEFAULT_IDENT;
}

int getLogFacility()
{
    char* ev = getenv("LOG_SYSLOG_FACILITY");

    if(ev && logFacilityMapping.find(ev) != logFacilityMapping.end())
    {
        return logFacilityMapping.at(ev);
    }
    return LOG_DEFAULT_FACILITY;
}

void log_init()
{
    int facility = getLogFacility();
    const char* ident = getLogIdent();
    openlog(ident, LOG_DEFAULT_OPTIONS, facility);
}

void log(Logger::Severity severity, const char* msg)
{
    switch(severity)
    {
    case Logger::Debug:     syslog(LOG_DEBUG,   "<" LOG_DEBUG_STR   "> : %s", msg);break;
    case Logger::Info:      syslog(LOG_INFO,    "<" LOG_INFO_STR    "> : %s", msg);break;
    case Logger::Warning:   syslog(LOG_WARNING, "<" LOG_WARNING_STR "> : %s", msg);break;
    case Logger::Error:     syslog(LOG_ERR,     "<" LOG_ERROR_STR   "> : %s", msg);break;
    case Logger::Fatal:     syslog(LOG_CRIT,    "<" LOG_CRIT_STR    "> : %s", msg);break;
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
