#ifndef LOG_ENTRY
#define LOG_ENTRY

#include<string>
#include<vector>
#include<ostream>

struct LogEntry
{
    std::string date;
    std::string time;
    std::string path;
    std::string message;
};
using LogEntries = std::vector<LogEntry>;

std::ostream& operator<<(std::ostream& os, const LogEntry& entry);

#endif