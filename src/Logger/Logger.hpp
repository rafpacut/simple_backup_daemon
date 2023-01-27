#ifndef LOGGER
#define LOGGER

#include<filesystem>
namespace fs = std::filesystem;

class Logger
{
    public:
    Logger(const fs::path& log_file_path) : log_file_path(log_file_path) {}
    void log_start(const fs::path& file_path) const;
    void log_finish(const fs::path& file_path) const;
    void log_error(const fs::path& file_path, const std::string& error_msg) const;


    private:
    void log_event(const fs::path& file_path, const std::string& message) const;
    const fs::path& log_file_path;
};

#endif