#include"InputParser/InputParser.hpp"
#include"BackgroundDaemonRunner/BackgroundDaemonRunner.hpp"
#include"UserInteractiveInputHandler/UserInteractiveInputHandler.hpp"
//todo here:
//app.log is a constant
//is_debug_run is taken from cmdline
//would be better if they were supplied in some sort of config file

int main(int argc, char* argv[])
{
    InputParser ip;
    const auto [is_debug_run, src_path, target_path] = ip.parse_cmdline_input(argc, argv);

    //for now
    const fs::path log_file_path = fs::current_path().append("app.log");

    BackupDaemonRunner daemon_runner(src_path, target_path, log_file_path);
    daemon_runner.run();

    if(not is_debug_run)
    {
        const UserInteractiveInputHandler input_handler(ip);
        input_handler.handle_log_display_queries(log_file_path);
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    daemon_runner.stop();

    return 0;
}