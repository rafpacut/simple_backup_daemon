#include"FsOperationWrapper.hpp"
#include<filesystem>
namespace fs = std::filesystem;

bool FsOperationWrapper::copy_file(const fs::path& from, const fs::path& to) const
{
    logger.log_start(from);
    const auto copy_result = fs::copy_file(from, to);
    logger.log_finish(from);

    return copy_result;
}