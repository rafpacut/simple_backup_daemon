#include<filesystem>
#include<system_error>
#include<thread>
#include"FsOperationWrapper.hpp"
#include"../utils/utils.hpp"
namespace fs = std::filesystem;

namespace
{
fs::path create_target_path_with_tmp_ext(const fs::path& path)
{
    auto path_tmp_ext = path;
    path_tmp_ext.replace_extension(path_tmp_ext.extension().string() + ".tmp");

    return path_tmp_ext;
}
}

void FsOperationWrapper::copy_file(const fs::path& source_path, const fs::path& target_path) const
{
    bool was_file_modified{false};
    if(fs::exists(target_path))
    {
        was_file_modified = fs::last_write_time(source_path) > fs::last_write_time(target_path);
    }
    const auto target_path_tmp = create_target_path_with_tmp_ext(target_path);
    try
    {
        fs::copy(source_path, target_path_tmp, fs::copy_options::update_existing);
        fs::copy(target_path_tmp, target_path, fs::copy_options::update_existing);
        fs::remove(target_path_tmp);
    }
    catch(const fs::filesystem_error& fs_error)
    {
        logger.log_error(fs_error.path1(), fs_error.what());
        return;
    }

    if(was_file_modified)
    {
        logger.log_modify(source_path);
        return;
    }
    logger.log_backup(source_path);
}

void FsOperationWrapper::remove(const fs::path& path) const
{
    try
    {
        fs::remove(path);
    }
    catch(const fs::filesystem_error& fs_error)
    {
        logger.log_error(fs_error.path1(), fs_error.what());
    }
    
    logger.log_remove(path);
}

void FsOperationWrapper::create_directory(const fs::path& new_dir_path) const
{
    try
    {
        fs::create_directory(new_dir_path);
    }
    catch(const fs::filesystem_error& fs_error)
    {
        logger.log_error(fs_error.path1(), fs_error.what());
    }
    logger.log_backup(new_dir_path);
}