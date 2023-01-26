#include "InputParser.hpp"

std::pair<fs::path, fs::path>
InputParser::parse_paths(int argc, char* argv[]) const
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
            throw std::runtime_error("Please provide existing src and target paths");
        }
        return {src_path, target_path};
    }
    throw std::runtime_error("Please provide existing src and target paths");
}