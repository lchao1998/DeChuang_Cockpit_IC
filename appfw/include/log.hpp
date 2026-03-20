/**
* @file         log.hpp
* @brief        log class
* @details      A severity logger
* @author   
* @date         2022-2-10
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention 
* 
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/02/10  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#ifndef APPFW_LOG_HPP
#define APPFW_LOG_HPP

#include <sstream>

namespace AppFw
{
class SeverityLog: public std::ostream
{
public:
    enum Severity{
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    SeverityLog(const Severity &severity);
    ~SeverityLog();

private:
    class Buffer : public std::streambuf {
    public:
        int_type overflow(int_type);
        std::streamsize xsputn(const char *, std::streamsize);
        std::stringstream data;
    };

private:
    int m_severity;
    Buffer m_buffer;
};
}

#define __LOG(severity) AppFw::SeverityLog(severity)

#define LOG_DEBUG	__LOG(AppFw::SeverityLog::DEBUG)
#define LOG_INFO	__LOG(AppFw::SeverityLog::INFO)
#define LOG_WARNING __LOG(AppFw::SeverityLog::WARNING)
#define LOG_ERROR	__LOG(AppFw::SeverityLog::ERROR)
#define LOG_FATAL	__LOG(AppFw::SeverityLog::FATAL)

#define SLOG_DEBUG
#define SLOG_INFO
#define SLOG_WARNING
#define SLOG_ERROR
#define SLOG_FATAL
#endif // APPFW_LOG_HPP
