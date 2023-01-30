#include"BackupDaemonRunner.hpp"

namespace
{
bool is_tagged_for_removal(const fs::directory_entry& entry)
{
    const auto filename_str = entry.path().filename().string();
    const auto delete_prefix_length = 7;
    if( filename_str.size() <= delete_prefix_length)
    {
        return false;
    }
    const auto filename_prefix = std::string(filename_str.begin(), filename_str.begin()+delete_prefix_length);
    return filename_prefix == "delete_";
}
}

void BackupDaemonRunner::run()
{
    for(const auto entry : fs::recursive_directory_iterator(src_base_path, fs::directory_options::follow_directory_symlink))
    {
        if(is_tagged_for_removal(entry))
        {
            const auto target_path = tpc.create_target_path_for_tagged_entry(entry);
            fs_op_wrap.remove(entry.path());
            fs_op_wrap.remove(target_path);
        }
        else if(entry.is_directory())
        {
            const auto target_path = tpc.create_target_path(entry.path());
            fs_op_wrap.create_directory(target_path);
        }
        else if(entry.is_symlink())
        {
            std::error_code error_code;
            const fs::path symlink_path = fs::read_symlink(entry.path(), error_code);//returns fs::path{} on error
            const auto target_path = tpc.create_target_path_for_symlink(entry.path(), entry.status());
            fs_op_wrap.copy_file(symlink_path, target_path);
        }
        else if(entry.is_regular_file())
        {
            const auto target_path = tpc.create_target_path_for_regular_file(entry.path());
            fs_op_wrap.copy_file(entry.path(), target_path);
        }
    }
}