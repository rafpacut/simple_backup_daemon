#include"BackgroundDaemonRunner.hpp"

void BackupDaemonRunner::run()
{
    daemon_thread_ptr = std::make_unique<std::thread>(backup_daemon, std::ref(running));
}

void BackupDaemonRunner::stop()
{
    running.store(false);
    daemon_thread_ptr->join();
}