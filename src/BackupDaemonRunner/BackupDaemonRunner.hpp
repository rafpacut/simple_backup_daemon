#include<filesystem>
#include"../Logger/Logger.hpp"
#include"../FsOperationWrapper/FsOperationWrapper.hpp"
#include"../TargetPathCreator/TargetPathCreator.hpp"
namespace fs = std::filesystem;

class BackupDaemonRunner
{
    public:
    BackupDaemonRunner(const fs::path& src_base_path, const fs::path& target_base_path, const fs::path& log_path) :
        src_base_path(src_base_path),
        target_base_path(target_base_path),
        logger(log_path),
        fs_op_wrap(logger),
        tpc(src_base_path, target_base_path) {}

    void run();

    private:
    FsOperationWrapper fs_op_wrap;
    Logger logger;
    TargetPathCreator tpc;

    const fs::path& src_base_path;
    const fs::path& target_base_path;
};