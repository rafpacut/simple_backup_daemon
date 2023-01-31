#include"AppLogReader.hpp"

std::vector<std::string> AppLogReader::split_by_comma(const std::string& line) const
{
    std::stringstream ss(line);
    std::string field;
    std::vector<std::string> row;
    while(std::getline(ss, field, ','))
    {
        row.push_back(field);
    }
    return row;
}

std::optional<LogEntry> AppLogReader::process_line(const std::string& line) const
{
    std::vector<std::string> row = split_by_comma(line);

    if(row.size() == LogEntry::FIELDS_NUM)
    {
        //que?
        return {{row[0],row[1],row[2],row[3]}};
    }
    return std::nullopt;
}

LogEntries AppLogReader::read_app_log(const fs::path& log_path) const
{
    //here apply check for log_path:
    //exists?
    //is_regular_file?
    //most likely acquire mutex (or discard incomplete entries)
    //open file
    LogEntries log_entries;

    std::ifstream log_file_stream;
    log_file_stream.open(log_path);

    std::string line;
    while(std::getline(log_file_stream, line))
    {
        const auto entry_opt = process_line(line);
        if(entry_opt)
        {
            log_entries.push_back(*entry_opt);
        }
    }
    return log_entries;
}