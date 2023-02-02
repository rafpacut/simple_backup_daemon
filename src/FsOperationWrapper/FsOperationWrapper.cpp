#include"FsOperationWrapper.hpp"
#include<filesystem>
#include<system_error>
namespace fs = std::filesystem;

namespace
{
fs::path create_target_path_tmp_ext(const fs::path& path)
{
    auto path_tmp_ext = path;
    path_tmp_ext.replace_extension(path_tmp_ext.extension().string() + ".tmp");

    return path_tmp_ext;
}

bool is_file_newer(const fs::path& src, const fs::path target)
{
    return fs::last_write_time(src) > fs::last_write_time(target);
}
}

void FsOperationWrapper::copy_file(const fs::path& source_path, const fs::path& target_path) const
{
    //uglyyyy name
    const bool file_modified = fs::exists(target_path);
    const auto target_path_tmp_ext = create_target_path_tmp_ext(target_path);
    if(fs::exists(target_path) and not is_file_newer(source_path, target_path))
    {
        return;
    }

    try
    {
        fs::copy(source_path, target_path_tmp_ext, fs::copy_options::update_existing);
        fs::copy(target_path_tmp_ext, target_path, fs::copy_options::update_existing);
        fs::remove(target_path_tmp_ext);
    }
    catch(const fs::filesystem_error& fs_error)
    {
        logger.log_error(fs_error.path1(), fs_error.what());
        return;
    }

    //uglyyy construct
    if(file_modified)
    {
        logger.log_modify(source_path);
        return;
    }
    logger.log_backup(source_path);
    /*
    const bool file_modified = fs::exists(target_path);
    try
    {
        fs::copy(source_path,
                target_path,
                fs::copy_options::update_existing);
    }
    catch(const fs::filesystem_error& fs_error)
    {
        logger.log_error(fs_error.path1(), fs_error.what());
        return;
    }

    if(file_modified)
    {
        logger.log_modify(source_path);
        return;
    }
    logger.log_backup(source_path);
    */
}

bool FsOperationWrapper::remove(const fs::path& path) const
{
    bool result{false};
    try
    {
        result = fs::remove(path);
    }
    catch(const fs::filesystem_error& fs_error)
    {
        logger.log_error(fs_error.path1(), fs_error.what());
    }
    
    logger.log_remove(path);
    return result;
}

bool FsOperationWrapper::create_directory(const fs::path& new_dir_path) const
{
    bool result{false}; 
    try
    {
        result = fs::create_directory(new_dir_path);
    }
    catch(const fs::filesystem_error& fs_error)
    {
        logger.log_error(fs_error.path1(), fs_error.what());
    }
    logger.log_backup(new_dir_path);
    return result;
}