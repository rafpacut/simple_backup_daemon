#ifndef UTILS
#define UTILS

#include<filesystem>

namespace utils
{
bool is_tmp_file(const fs::directory_entry&);
bool is_file_newer(const fs::path&, const fs::path&);
fs::path strip_tmp_ext(const fs::path&);
fs::path strip_bak_ext(const fs::path&);
}

#endif