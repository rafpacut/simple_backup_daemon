#include"InputParser/InputParser.hpp"
#include"FsScanner/FsScanner.hpp"


int main(int argc, char* argv[])
{
    InputParser ip;
    const auto [src_path, target_path] = ip.parse_paths(argc, argv);
    FsScanner scanner(src_path, target_path);
    scanner.scan();
    return 0;
}