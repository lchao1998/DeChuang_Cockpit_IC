#ifndef LOG_BASE_HPP
#define LOG_BASE_HPP

#include <string>
#include <ostream>

class Logger
{
public:
    enum Severity
    {
        Debug = 1,
        Info,
        Warning,
        Error,
        Fatal
    };

    explicit Logger(Severity severity);

    ~Logger();

    template <typename T>
    std::ostream& operator <<(T d)
    {
        return (m_stream << d);
    }

    static std::string severityToString(Severity severity);

private:
    Severity m_severity;
    std::ostream& m_stream;

private:
    static Severity m_severityFilter;
};

extern void log_init();
extern void log(Logger::Severity severity, const char* msg);
extern void log_f(Logger::Severity severity, const char* format, ...) __attribute__ ((format (printf, 2, 3)));
extern void log_v(Logger::Severity severity, const char* format, va_list args) __attribute__ ((__format__ (__printf__, 2, 0)));

#endif
