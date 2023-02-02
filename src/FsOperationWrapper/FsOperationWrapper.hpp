#ifndef FS_OPERATION_WRAPPER 
#define FS_OPERATION_WRAPPER

#include<filesystem>
#include"../Logger/Logger.hpp"
namespace fs = std::filesystem;

class FsOperationWrapper
{
    public:
    FsOperationWrapper(const Logger& logger) : logger(logger) {}

    void copy_file(const fs::path&, const fs::path&) const;
    void create_directory(const fs::path&) const;
    void remove(const fs::path&) const;

    private:
    const Logger& logger;
};

#endif