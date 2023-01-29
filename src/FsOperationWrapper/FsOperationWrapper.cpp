#include"FsOperationWrapper.hpp"
#include<filesystem>
#include<system_error>
#include<iostream>
namespace fs = std::filesystem;

bool FsOperationWrapper::copy_file(const fs::path& source_path, const fs::path& target_path) const
{
    const bool file_modified = fs::exists(target_path);

    std::error_code error_code;
    fs::copy(source_path,
            target_path,
            fs::copy_options::update_existing,
            error_code);
    if(error_code)
    {
        logger.log_error(source_path, error_code.message());
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
    std::error_code error_code;
    const bool completed = fs::remove(path, error_code);
    if(not completed and error_code.value() != 0)
    {
        logger.log_error(path, error_code.message());
        return completed;
    }
    logger.log_remove(path);
    return completed;

}

bool FsOperationWrapper::create_directory(const fs::path& new_dir_path) const
{
    std::error_code error_code;
    const bool completed = fs::create_directory(new_dir_path, error_code);
    if(not completed)
    {
        logger.log_error(new_dir_path, error_code.message());
        return completed;
    }
    logger.log_backup(new_dir_path);
    return completed;
}