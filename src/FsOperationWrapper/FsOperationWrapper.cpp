#include"FsOperationWrapper.hpp"
#include<filesystem>
#include<system_error>
namespace fs = std::filesystem;

bool FsOperationWrapper::copy_file(const fs::path& from, const fs::path& to) const
{
    logger.log_start(from);

    std::error_code error_code;
    fs::copy(from, to,
        fs::copy_options::update_existing | fs::copy_options::recursive,
        error_code);


    if(error_code)
    {
        logger.log_error(from, error_code.message());
        return false;
    }
    logger.log_finish(from);
    return true;
}


bool FsOperationWrapper::create_directory(const fs::path& new_dir_path) const
{
    std::error_code error_code;
    logger.log_start(new_dir_path);
    const bool completed = fs::create_directory(new_dir_path, error_code);
    if(not completed)
    {
        logger.log_error(new_dir_path, error_code.message());
        return completed;
    }
    logger.log_finish(new_dir_path);
    return completed;
}