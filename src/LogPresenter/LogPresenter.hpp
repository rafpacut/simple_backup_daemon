#ifndef LOG_PRESENTER
#define LOG_PRESENTER

#include<iostream>
#include<regex>
#include<iterator>
#include<filesystem>
#include"../LogEntry/LogEntry.hpp"
namespace fs = std::filesystem;

class LogPresenter
{
    public:
        LogPresenter() = default;
        void display_entries(const LogEntries&) const;
        void display_entries(const LogEntries&, std::regex&, std::regex&) const;

    private:
        LogEntries filter_entries(const LogEntries&, std::regex&, std::regex&) const;
};

#endif