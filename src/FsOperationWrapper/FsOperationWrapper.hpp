#ifndef FS_OPERATION_WRAPPER 
#define FS_OPERATION_WRAPPER

#include<filesystem>
#include"../Logger/Logger.hpp"
namespace fs = std::filesystem;

class FsOperationWrapper
{
    public:
    FsOperationWrapper(const Logger& logger) : logger(logger) {}

    bool copy_file(const fs::path&, const fs::path&) const;
    bool create_directory(const fs::path&) const;
    bool remove(const fs::path&) const;

    private:
    const Logger& logger;
};

#endif