#include<filesystem>
#include<atomic>
#include<unordered_map>
#include"../Logger/Logger.hpp"
#include"../FsOperationWrapper/FsOperationWrapper.hpp"
#include"../TargetPathCreator/TargetPathCreator.hpp"
namespace fs = std::filesystem;

class BackupDaemon
{
    public:
        BackupDaemon(const fs::path& src_base_path
                    , const fs::path& target_base_path
                    , const fs::path& log_path)
        : src_base_path(src_base_path)
        , target_base_path(target_base_path)
        , logger(log_path)
        , fs_op_wrap(logger)
        , tpc(src_base_path, target_base_path, logger) {}

        void operator()(std::atomic_bool&);

    private:
        void resume_interrupted_copies() const;
        void backup_entry(const fs::directory_entry& entry);

        bool is_tagged_for_removal(const fs::directory_entry& entry) const;
        void remove_src_file_without_tag(const fs::path& tag_file_path) const;

        FsOperationWrapper fs_op_wrap;
        Logger logger;
        TargetPathCreator tpc;

        const fs::path& src_base_path;
        const fs::path& target_base_path;

        const std::chrono::milliseconds DELAY{200};
        const std::string DELETE_PREFIX{"delete_"};
        const size_t DELETE_PREFIX_LENGTH{7};

        using source_path_string = std::string;
        using target_path_string = std::string;
        std::unordered_map<source_path_string, target_path_string> path_cache;
};