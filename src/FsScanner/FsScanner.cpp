#include"FsScanner.hpp"

void FsScanner::scan() const
{
    for(const auto& entry : std::filesystem::recursive_directory_iterator(src_path))
    {
        if(entry.is_regular_file())
        {
            fs::path target_file_path = target_path;
            target_file_path.append(entry.path().filename().string());
            target_file_path.replace_extension(entry.path().extension().string() + ".bak");

            fs::copy_file(entry.path(), target_file_path);
        }
    }
}