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

    std::pair<fs::path, fs::path>
    parse_paths(int argc, char* argv[]) const;

    //std::pair<std::regex,std::regex> parse_log_regex(const std::vector<std::string>&) const;
    std::optional<RegexPair> parse_log_regex(const std::string&) const;
};