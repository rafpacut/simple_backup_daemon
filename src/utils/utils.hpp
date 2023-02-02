#ifndef UTILS
#define UTILS

#include<filesystem>

namespace utils
{
bool is_tmp_file(const fs::directory_entry&);
fs::path strip_tmp_ext(const fs::path&);
fs::path strip_bak_ext(const fs::path&);
}

#endif