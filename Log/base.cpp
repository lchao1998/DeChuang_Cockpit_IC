#include "base.hpp"
#include <map>
#include <mutex>

static const std::map<std::string, Logger::Severity> SeverityStrMapping =
{
    {"debug",   Logger::Debug},
    {"info",    Logger::Info},
    {"warning", Logger::Warning},
    {"error",   Logger::Error},
    {"fatal",   Logger::Fatal},
};

static const Logger::Severity DefaultSeverityFilter = Logger::Debug;

Logger::Severity getSeverityFilter()
{
    char* ev = getenv("LOG_SEVERITY_FILTER");

    if(ev && SeverityStrMapping.find(ev) != SeverityStrMapping.end())
    {
        return SeverityStrMapping.at(ev);
    }
    return DefaultSeverityFilter;
}


class LogStreamBuffer : public std::streambuf
{
public:
    explicit LogStreamBuffer(Logger::Severity severity):
        m_severity(severity)
    {}

protected:
    int_type overflow(int_type c)
    {
        if (c != EOF) {
            m_data.push_back(c);
        }
        return (c);
    }

    int sync()
    {
        log(m_severity, m_data.c_str());
        m_data.clear();
        return 0;
    }

private:
    Logger::Severity m_severity;
    std::string m_data;
};

template<Logger::Severity Severity>
class LogStream: public std::ostream
{
public:
    static LogStream& get()
    {
        static LogStream stream;
        return stream;
    }

    LogStream():
        m_buf(new LogStreamBuffer(Severity)),
        std::ostream()
    {
        init(m_buf);
    }

    ~LogStream()
    {
        if(m_buf)
            delete m_buf;
    }

private:
    LogStreamBuffer* m_buf;
};

static std::mutex logMtx;

static std::ostream nullStream(NULL);

static std::ostream& getLogStream(Logger::Severity severity)
{
    static bool init = false;

    if(!init)
    {
        log_init();
        init = true;
    }

    switch (severity) {
    case Logger::Debug:     return LogStream<Logger::Debug>::get();
    case Logger::Info:      return LogStream<Logger::Info>::get();
    case Logger::Warning:   return LogStream<Logger::Warning>::get();
    case Logger::Error:     return LogStream<Logger::Error>::get();
    case Logger::Fatal:     return LogStream<Logger::Fatal>::get();
    }
    return nullStream;
}

Logger::Severity Logger::m_severityFilter = getSeverityFilter();

Logger::Logger(Severity severity):
    m_severity(severity),
    m_stream((severity>= m_severityFilter) ? getLogStream(severity) : nullStream)
{
    if(m_stream.rdbuf())
        logMtx.lock();
}

Logger::~Logger()
{
    if(m_stream.rdbuf())
    {
        m_stream.flush();
        logMtx.unlock();
    }
}

std::string Logger::severityToString(Severity severity)
{
    switch (severity) {
    case Severity::Debug:   return "debug";
    case Severity::Info:    return "info";
    case Severity::Warning: return "warning";
    case Severity::Error:   return "error";
    case Severity::Fatal:   return "fatal";
    }
    return "unknown";
}
