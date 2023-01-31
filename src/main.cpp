#include<thread>
#include<iostream>
#include"InputParser/InputParser.hpp"
#include"BackupDaemonRunner/BackupDaemonRunner.hpp"
#include"UserInteractiveInputHandler/UserInteractiveInputHandler.hpp"

int main(int argc, char* argv[])
{
    InputParser ip;
    const auto [src_path, target_path] = ip.parse_paths(argc, argv);

    //for now
    const fs::path log_file_path = fs::current_path().append("app.log");

    BackupDaemonRunner backup_daemon(src_path, target_path, log_file_path);
    //std::thread daemon_thread(backup_daemon);
    //daemon_thread.detach();
    backup_daemon();

    /*
    const UserInteractiveInputHandler input_handler(ip);
    input_handler.handle_log_display_queries(log_file_path);
    */

    return 0;
}