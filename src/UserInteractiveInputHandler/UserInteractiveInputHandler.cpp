#include"UserInteractiveInputHandler.hpp"

void UserInteractiveInputHandler::handle_log_display_queries(const fs::path& log_file_path) const
{
    std::string line;

    bool running{true};
    while(running)
    {
        std::cout<<prompt;
        std::getline(std::cin, line);
        
        if(line.size() == 0)
        {
            auto log_entries = log_reader.read_app_log(log_file_path);
            presenter.display_entries(log_entries);
            continue;
        }

        if(ip.try_parse_quit(line))
        {
            break;
        }

        std::optional<RegexPair> regex_pair_opt= ip.parse_log_regex(line);
        while(not regex_pair_opt)
        {
            std::cout<<prompt;
            std::getline(std::cin, line);
            regex_pair_opt =  ip.parse_log_regex(line);
        }

        auto [date_regex, filename_regex] = *regex_pair_opt;
        auto log_entries = log_reader.read_app_log(log_file_path);
        presenter.display_entries(log_entries, date_regex, filename_regex);
    }
}