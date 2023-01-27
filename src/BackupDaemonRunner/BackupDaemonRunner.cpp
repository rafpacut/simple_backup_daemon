#include"BackupDaemonRunner.hpp"
namespace
{
fs::path create_target_path(const fs::path& filepath,
                            const fs::path& target_base_path,
                            const fs::path& src_base_path)
{
   const auto base_src_path_length = src_base_path.string().length();
   const auto filepath_str = filepath.string();
   const auto path_relative_to_source_base_path = std::string(filepath_str.begin()+base_src_path_length,
                                                                filepath_str.end());
   const auto relative_path = fs::path(path_relative_to_source_base_path);
   const auto fullpath_str = target_base_path.string() + relative_path.string();
   return fs::path(fullpath_str);
}

fs::path create_target_file_path(const fs::path& src_path, const fs::path& target_base_path, const fs::path& src_base_path)
{
    auto target_file_path = create_target_path(src_path, target_base_path, src_base_path);
    target_file_path.replace_extension(src_path.extension().string() + ".bak");
    return target_file_path;
}
}

void BackupDaemonRunner::run()
{
    for(const auto& entry : std::filesystem::recursive_directory_iterator(src_base_path))
    {
        if(entry.is_regular_file())
        {
            const fs::path target_file_path = create_target_file_path(entry.path(), target_base_path, src_base_path);
            fs_op_wrap.copy_file(entry.path(), target_file_path);
        }

        if(entry.is_directory())
        {
            const fs::path target_dir_path = create_target_path(entry.path(), target_base_path, src_base_path);
            fs_op_wrap.create_directory(target_dir_path);
        }
    }
}