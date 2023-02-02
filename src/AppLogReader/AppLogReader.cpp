#include"AppLogReader.hpp"
#include<fstream>
#include<sstream>

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
        //que? Why do I need FIELDS_NUM then?
        return {{row[0],row[1],row[2],row[3]}};
    }
    return std::nullopt;
}

LogEntries AppLogReader::read_app_log(const fs::path& log_path) const
{
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
    log_file_stream.close();
    return log_entries;
}