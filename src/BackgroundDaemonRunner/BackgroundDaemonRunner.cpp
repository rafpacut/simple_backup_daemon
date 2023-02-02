#include"BackgroundDaemonRunner.hpp"
#include<iostream>

void BackupDaemonRunner::run()
{
    daemon_thread_ptr = std::make_unique<std::thread>(backup_daemon, std::ref(running));
}

void BackupDaemonRunner::stop()
{
    std::cout<<"waiting for backup to finish"<<std::endl;
    running.store(false);
    daemon_thread_ptr->join();
    std::cout<<"finished"<<std::endl;
}