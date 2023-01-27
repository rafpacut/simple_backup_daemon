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
    //errno_t err = localtime_s(&tstruct, &now);

    strftime(buf, sizeof(buf), "%Y-%m-%d,%X", &tstruct);
    return buf;
}
}

void Logger::log_start(const fs::path& file_path) const
{
    log_event(file_path, "started backing up");
}

void Logger::log_finish(const fs::path& file_path) const
{
    log_event(file_path, "finished backing up");
}

void Logger::log_error(const fs::path& file_path, const std::string& error_msg) const
{
    log_event(file_path, error_msg);
}

void Logger::log_event(const fs::path& file_path, const std::string& message) const
{
    std::string time_date_now = get_date_time_now();
    std::ofstream ofs(log_file_path.string(), std::ofstream::app);
    ofs<<time_date_now<<","<<file_path.filename().string()<<","<<message<<std::endl;
    ofs.close();
}