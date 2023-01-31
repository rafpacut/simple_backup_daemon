#ifndef APP_LOG_READER
#define APP_LOG_READER
#include<filesystem>
#include<vector>
#include<string>
#include<optional>
#include<fstream>
#include<sstream>
#include"../LogEntry/LogEntry.hpp"
namespace fs = std::filesystem;

class AppLogReader
{
    public:
        AppLogReader() = default;
        LogEntries read_app_log(const fs::path&) const;

    private:
        std::vector<std::string> split_by_comma(const std::string&) const;
        std::optional<LogEntry> process_line(const std::string&) const;
};
#endif