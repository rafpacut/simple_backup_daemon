#include"FsOperationWrapper.hpp"
#include<filesystem>
#include<system_error>
#include<iostream>
namespace fs = std::filesystem;

bool FsOperationWrapper::copy_file(const fs::path& source_path, const fs::path& target_path) const
{
    const bool file_modified = fs::exists(target_path);

    try
    {
        fs::copy(source_path,
                target_path,
                fs::copy_options::update_existing);
    }catch(const fs::filesystem_error& fs_error)
    {
        logger.log_error(fs_error.path1(), fs_error.what());
        return false;
    }

    if(file_modified)
    {
        logger.log_modify(source_path);
    }
    else
    {
        logger.log_backup(source_path);
    }
    return true;
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