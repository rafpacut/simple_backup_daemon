#include"LogPresenter.hpp"

void LogPresenter::display_entries(const LogEntries& log_entries) const
{
    std::cout<<"APP LOG:\n";
    for(const auto& entry: log_entries)
    {
        std::cout<<entry<<std::endl;
    }
}

void LogPresenter::display_entries(const LogEntries& log_entries, std::regex& date_regex, std::regex& filename_regex) const
{
    const auto filtered_entries = filter_entries(log_entries, date_regex, filename_regex);
    display_entries(filtered_entries);
}

LogEntries LogPresenter::filter_entries(const LogEntries& log_entries, std::regex& date_regex, std::regex& filename_regex) const
{
    LogEntries filtered_entries;

    std::copy_if(log_entries.cbegin(), log_entries.cend(), std::back_inserter(filtered_entries),
    [date_regex, filename_regex](const LogEntry& entry)
    {
        const auto filename = fs::path{entry.path}.filename().string();//always matching both regexes

        return std::regex_match(entry.date, date_regex)
            and std::regex_match(filename, filename_regex);
    });

    return filtered_entries;
}