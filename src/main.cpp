#include<utility>
#include<string>
#include<stdexcept>
#include<vector>
#include<iostream>
#include<filesystem>
namespace fs = std::filesystem;

class InputParser
{
public:
    InputParser() = default;

    std::pair<fs::path, fs::path>
    parse_paths(int argc, char* argv[]) const
    {
        if(argc == 3)
        {
            fs::path src_path;
            fs::path target_path;
            try{
                src_path = std::string(argv[1]);
                target_path = std::string(argv[2]);
            }
            catch(std::runtime_error& e)
            {
                std::cerr<<e.what()<<std::endl;
                throw std::runtime_error("FFOOPlease provide existing src and target paths");
            }
            return {src_path, target_path};
        }
        throw std::runtime_error("Please provide existing src and target paths");
    }
};

int main(int argc, char* argv[])
{
    InputParser ip;
    const auto [src_path, target_path] = ip.parse_paths(argc, argv);
    for(const auto& entry : std::filesystem::recursive_directory_iterator(src_path))
    {
        //std::cout<<"traversing "<<entry.path()<<std::endl;
        if(entry.is_regular_file())
        {
            fs::path target_file_path = target_path;
            target_file_path.append(entry.path().filename().string());
            target_file_path.replace_extension(entry.path().extension().string() + ".bak");

            fs::copy_file(entry.path(), target_file_path);
        }
    }
    return 0;
}