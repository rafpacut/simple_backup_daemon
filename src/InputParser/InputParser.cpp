#include"InputParser.hpp"
#include<iostream>

namespace
{
    void print_help_msg()
    {
        std::cout<<"Usage: ./scanner <source_director_path> <target_directory_path>\n";
    }
}

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
    throw std::runtime_error("Please provide src and target paths");
}

std::optional<RegexPair>
InputParser::parse_log_regex(const std::string& line) const
{
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> words;
    while(std::getline(ss, word, ' '))
    {
        if(word.size() > 0)
        {
            words.push_back(word);
        }
    }

    if(words.size() != 2)
    {
        std::cerr<<"Please provide two valid regex entries: <date_regex> and <filename_regex>"<<std::endl;
        return std::nullopt;
    }

    std::regex date_regex, filename_regex;
    try
    {
        date_regex = std::regex(words[0]);
        filename_regex = std::regex(words[1]);
    }
    catch(const std::regex_error& e)
    {
        std::cerr<<e.what();
        return std::nullopt;
    }

    return {{date_regex, filename_regex}};
}




















