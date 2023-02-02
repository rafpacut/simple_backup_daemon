#ifndef UTILS
#define UTILS

#include"utils.hpp"
#include<filesystem>

namespace fs = std::filesystem;


namespace utils
{
bool is_tmp_file(const fs::directory_entry& entry)
{
    if(not entry.is_regular_file())
    {
        return false;
    } 

    const auto extension = entry.path().extension().string();
    const std::string tmp_extension = ".tmp";
    return (extension == tmp_extension);
}

fs::path strip_bak_ext(const fs::path& path)
{
    auto path_no_bak = path;
    const auto filename = path.filename().string();

    const auto tmp_ext_length = 4;
    if(filename.size() > tmp_ext_length)
    {
        const auto filename_suffix = std::string(filename.end()-tmp_ext_length, filename.end());
        path_no_bak.replace_filename(std::string(filename.begin(), filename.end()-tmp_ext_length));
    }
    return path_no_bak;
}

fs::path strip_tmp_ext(const fs::path& path)
{
    auto path_no_tmp = path;
    //const auto path_ext = path.extension().string();
    const auto filename = path.filename().string();
    const auto tmp_ext_length = 4;
    if(filename.size() > tmp_ext_length)
    {
        const auto filename_suffix = std::string(filename.end()-tmp_ext_length, filename.end());
        if(filename_suffix == ".tmp")
        {
            path_no_tmp.replace_filename(std::string(filename.begin(), filename.end()-tmp_ext_length));
        }
    }
    //const auto path_ext_no_tmp = std::string(path_ext.begin(), path_ext.begin()+tmp_ext_length);
    //path_no_tmp.replace_extension(path_ext_no_tmp);

    return path_no_tmp;
}
}

#endif