#include<filesystem>
#include"../Logger/Logger.hpp"
#include"../FsOperationWrapper/FsOperationWrapper.hpp"
#include"../TargetPathCreator/TargetPathCreator.hpp"
namespace fs = std::filesystem;

class BackupDaemonRunner
{
    public:
        BackupDaemonRunner(const fs::path& src_base_path
                         , const fs::path& target_base_path
                         , const fs::path& log_path)
        : src_base_path(src_base_path)
        , target_base_path(target_base_path)
        , logger(log_path)
        , fs_op_wrap(logger)
        , tpc(src_base_path, target_base_path, logger) {}

        void operator()();

    private:
        bool is_tagged_for_removal(const fs::directory_entry& entry) const;
        void remove_src_file_without_tag(const fs::path& tag_file_path) const;

        FsOperationWrapper fs_op_wrap;
        Logger logger;
        TargetPathCreator tpc;

        const fs::path& src_base_path;
        const fs::path& target_base_path;

        const std::string DELETE_PREFIX = "delete_";
        const size_t DELETE_PREFIX_LENGTH = 7;
};