#include<filesystem>
namespace fs = std::filesystem;


class FsScanner
{
    public:
    FsScanner(const fs::path& src, const fs::path& target) : src_path(src), target_path(target) {}

    void scan() const;

    private:
    const fs::path& src_path;
    const fs::path& target_path;
};