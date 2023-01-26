#include<utility>
#include<stdexcept>
#include<filesystem>
#include<iostream>

namespace fs = std::filesystem;

class InputParser
{
public:
    InputParser() = default;

    std::pair<fs::path, fs::path>
    parse_paths(int argc, char* argv[]) const;
};