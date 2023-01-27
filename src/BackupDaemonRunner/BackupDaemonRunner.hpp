#include<filesystem>
#include"../Logger/Logger.hpp"
#include"../FsOperationWrapper/FsOperationWrapper.hpp"
namespace fs = std::filesystem;

class BackupDaemonRunner
{
    public:
    BackupDaemonRunner(const fs::path& src_path, const fs::path& target_path, const fs::path& log_path) :
        src_path(src_path),
        target_path(target_path),
        logger(log_path),
        fs_op_wrap(logger) {}

    void run();

    private:
    FsOperationWrapper fs_op_wrap;
    Logger logger;

    const fs::path& src_path;
    const fs::path& target_path;
};