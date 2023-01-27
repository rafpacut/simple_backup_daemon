#ifndef FS_OPERATION_WRAPPER 
#define FS_OPERATION_WRAPPER

#include<filesystem>
#include"../Logger/Logger.hpp"
namespace fs = std::filesystem;

class FsOperationWrapper
{
    public:
    FsOperationWrapper(const Logger& logger) : logger(logger) {}

    bool copy_file(const fs::path& from, const fs::path& to) const;

    private:
    const Logger& logger;
};

#endif