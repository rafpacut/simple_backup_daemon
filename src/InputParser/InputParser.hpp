#ifndef INPUT_PARSER
#define INPUT_PARSER

#include<utility>
#include<stdexcept>
#include<filesystem>
#include<regex>
#include<optional>

namespace fs = std::filesystem;
using RegexPair = std::pair<std::regex, std::regex>;

class InputParser
{
public:
    InputParser() = default;

    std::tuple<bool, fs::path, fs::path>
    parse_cmdline_input(int argc, char* argv[]) const;

    std::optional<RegexPair> parse_log_regex(const std::string&) const;
    bool try_parse_quit(const std::string& line) const;

    private:

    const std::string debug_run_symbol = "d";
    std::vector<std::string> split_line_by_whitespace(const std::string& line) const;
};

#endif