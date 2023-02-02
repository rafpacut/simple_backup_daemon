#include"InputParser.hpp"
#include<iostream>


std::tuple<bool, fs::path, fs::path>
InputParser::parse_cmdline_input(int argc, char* argv[]) const
{
    bool is_debug_run{false};
    fs::path src_path;
    fs::path target_path;
    if(argc == 4 and std::string(argv[3]) == DEBUG_RUN_SYMBOL)
    {
        is_debug_run = true;
    }
    if((argc == 3) or is_debug_run)
    {
        try
        {
            src_path = fs::path(argv[1]);
            target_path = fs::path(argv[2]);
        }
        catch(std::runtime_error& e)
        {
            std::cerr<<e.what()<<std::endl;
            throw std::runtime_error("Please provide existing src and target paths");
        }
        return {is_debug_run, src_path, target_path};
    }
    throw std::runtime_error("Please provide src and target paths");
}

std::optional<RegexPair>
InputParser::parse_log_regex(const std::string& line) const
{
    const std::vector<std::string> words = split_line_by_whitespace(line);
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

bool InputParser::try_parse_quit(const std::string& line) const
{
    const std::vector<std::string> words = split_line_by_whitespace(line);
    return (words.size() == 1) and words[0] == "q";
}

std::vector<std::string> InputParser::split_line_by_whitespace(const std::string& line) const
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
    return words;
}


















