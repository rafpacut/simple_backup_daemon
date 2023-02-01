#ifndef BACKGROUND_DAEMON_RUNNER
#define BACKGROUND_DAEMON_RUNNER

#include<atomic>
#include<thread>
#include"BackupDaemon/BackupDaemon.hpp"

class BackupDaemonRunner
{
    public:
    BackupDaemonRunner(const fs::path& src_path
    , const fs::path& target_path
    , const fs::path& log_file_path)
    : backup_daemon(src_path, target_path, log_file_path) {}

    void run();
    void stop();

    private:
    BackupDaemon backup_daemon;
    std::unique_ptr<std::thread> daemon_thread_ptr{nullptr};
    std::atomic_bool running{true};
};

void BackupDaemonRunner::run()
{
    daemon_thread_ptr = std::make_unique<std::thread>(backup_daemon, std::ref(running));
}

void BackupDaemonRunner::stop()
{
    running.store(false);
    daemon_thread_ptr->join();
}

#endif