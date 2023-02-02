#include"InputParser/InputParser.hpp"
#include"BackgroundDaemonRunner/BackgroundDaemonRunner.hpp"
#include"UserInteractiveInputHandler/UserInteractiveInputHandler.hpp"

namespace
{
void print_prompt()
{
    std::cout<<"Usage: scanner /path/to/src/directory /path/to/target/directory\n";
}
}


int main(int argc, char* argv[])
{
    InputParser ip;
    bool is_debug_run{false};
    fs::path src_path;
    fs::path target_path;
    try
    {
        std::tie(is_debug_run, src_path, target_path) = ip.parse_cmdline_input(argc, argv);
    }
    catch(...)
    {
        print_prompt();
        exit(0);
    }

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