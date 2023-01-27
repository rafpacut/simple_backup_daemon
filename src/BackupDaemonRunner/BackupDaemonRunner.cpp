#include"BackupDaemonRunner.hpp"

namespace
{
fs::path create_target_file_path(const fs::directory_entry& entry_to_copy, const fs::path target_path)
{
    fs::path target_file_path = target_path;
    target_file_path.append(entry_to_copy.path().filename().string());
    target_file_path.replace_extension(entry_to_copy.path().extension().string() + ".bak");
    return target_file_path;
}
}

void BackupDaemonRunner::run()
{
    for(const auto& entry : std::filesystem::recursive_directory_iterator(src_path))
    {
        if(entry.is_regular_file())
        {
            const fs::path target_file_path = create_target_file_path(entry, target_path);
            fs_op_wrap.copy_file(entry.path(), target_file_path);
        }
    }
}