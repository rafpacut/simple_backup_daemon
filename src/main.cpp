#include"InputParser/InputParser.hpp"
#include"BackupDaemonRunner/BackupDaemonRunner.hpp"


int main(int argc, char* argv[])
{
    InputParser ip;
    const auto [src_path, target_path] = ip.parse_paths(argc, argv);

    //for now
    const fs::path log_file_path = fs::current_path().append("app.log");

    BackupDaemonRunner backup_daemon(src_path, target_path, log_file_path);
    backup_daemon.run();
    return 0;
}