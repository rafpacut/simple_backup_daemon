#include"Logger.hpp"
#include<fstream>
#include <stdio.h>
#include <time.h>

namespace
{
std::string get_date_time_now()
{
    struct tm tstruct;
    char buf[80];
    time_t now = time(0);
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d,%X", &tstruct);
    return buf;
}
}

void Logger::log_modify(const fs::path& path) const
{
    log_event(path, "modified");
}

void Logger::log_backup(const fs::path& path) const
{
    log_event(path, "backed up");
}

void Logger::log_remove(const fs::path& path) const
{
    log_event(path, "removed");
}

void Logger::log_error(const fs::path& path, const std::string& error_msg) const
{
    log_event(path, "ERR: "+error_msg);
}

void Logger::log_event(const fs::path& path, const std::string& message) const
{
    std::string time_date_now = get_date_time_now();
    std::ofstream ofs(log_file_path.string(), std::ofstream::app);
    ofs<<time_date_now<<','<<path.string()<<','<<message<<std::endl;
    ofs.close();
}