#include "log.hpp"
#include <chrono>
#include "configuration/configuration.hpp"
#include <iostream>
#include <iomanip>
#include <mutex>

namespace AppFw
{

static std::mutex mtx;
static int line_id = 0;

SeverityLog::SeverityLog(const SeverityLog::Severity &severity):
    m_severity(severity),
    std::ostream(&m_buffer)
{
}

SeverityLog::~SeverityLog()
{
    if (m_severity >= Configuration::get().logSeverity)
    {
        const char* severityName = 0;
        switch (m_severity)
        {
        case SeverityLog::DEBUG:	severityName = "DEBUG";     break;
        case SeverityLog::INFO:		severityName = "INFO";      break;
        case SeverityLog::WARNING:  severityName = "WRANING";   break;
        case SeverityLog::ERROR:	severityName = "ERROR";     break;
        case SeverityLog::FATAL:    severityName = "FATAL";     break;
        default:                    severityName = "-";         break;
        }

        auto now = std::chrono::steady_clock::now();
        auto timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

        std::lock_guard<std::mutex> locker(mtx);
        std::cout << std::dec << std::setw(4) << std::setfill('0') << ++line_id << " | "
                << std::dec << std::setw(8) << std::setfill('0') << timeStamp << " | "
                << severityName << " | "
               <<m_buffer.data.str()
              <<std::endl;
    }
}

std::streambuf::int_type SeverityLog::Buffer::overflow(std::streambuf::int_type c)
{
    if (c != EOF) {
        data << (char)c;
    }

    return (c);
}

std::streamsize SeverityLog::Buffer::xsputn(const char *s, std::streamsize n)
{
    data.write(s, n);
    return (n);
}
}
