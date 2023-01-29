#ifndef LOGGER
#define LOGGER

#include<filesystem>
namespace fs = std::filesystem;

class Logger
{
    public:
    Logger(const fs::path& log_file_path) : log_file_path(log_file_path) {}
    void log_backup(const fs::path&) const;
    void log_modify(const fs::path&) const;
    void log_remove(const fs::path&) const;
    void log_error(const fs::path&, const std::string&) const;


    private:
    void log_event(const fs::path&, const std::string&) const;
    const fs::path& log_file_path;
};

#endif