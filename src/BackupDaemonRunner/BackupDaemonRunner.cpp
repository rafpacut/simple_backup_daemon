#include"BackupDaemonRunner.hpp"
namespace
{
fs::path get_path_relative_to(const fs::path& src_base_path, const fs::path& filepath)
{
   const auto base_src_path_length = src_base_path.string().length();
   const auto filepath_str = filepath.string();
   const auto path_relative_to_source_base_path = std::string(filepath_str.begin()+base_src_path_length,
                                                                filepath_str.end());
   return fs::path(path_relative_to_source_base_path);
}

fs::path create_target_path(const fs::path& filepath,
                            const fs::path& target_base_path,
                            const fs::path& src_base_path)
{
    const auto relative_path = get_path_relative_to(src_base_path, filepath);
    const auto fullpath_str = target_base_path.string() + relative_path.string();
    return fs::path(fullpath_str);
}

fs::path create_target_file_path(const fs::path& filepath,
                                 const fs::path& target_base_path,
                                 const fs::path& src_base_path)
{
    fs::path target_path = create_target_path(filepath, target_base_path, src_base_path);
    target_path.replace_extension(target_path.extension().string() + ".bak");
    return target_path;
}

std::pair<fs::path,fs::path>
create_target_path_for_symlink(const fs::path& filepath,
                               const fs::path& target_base_path,
                               const fs::path& src_base_path)
{
    fs::path target_path = filepath;
    const fs::path symlink_path = fs::read_symlink(filepath);
    target_path.replace_filename(symlink_path.filename());
    target_path.replace_extension(symlink_path.extension());

    target_path = create_target_file_path(target_path, target_base_path, src_base_path);
    return {target_path, symlink_path};
}
}

void BackupDaemonRunner::run()
{
    for(const auto& entry : std::filesystem::recursive_directory_iterator(src_base_path, fs::directory_options::follow_directory_symlink))
    {
        if(entry.is_directory())
        {
            const fs::path target_path = create_target_path(entry.path(), target_base_path, src_base_path);
            fs_op_wrap.create_directory(target_path);
        }
        else if(entry.is_symlink())
        {
            auto [target_path, symlink_path] = create_target_path_for_symlink(entry.path(), target_base_path, src_base_path);
            fs_op_wrap.copy_file(symlink_path, target_path);
        }
        else if(entry.is_regular_file())
        {
            const fs::path target_path = create_target_file_path(entry.path(), target_base_path, src_base_path);
            fs_op_wrap.copy_file(entry.path(), target_path);
        }
    }
}