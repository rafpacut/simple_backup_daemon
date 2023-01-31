#include"LogEntry.hpp"
std::ostream& operator<<(std::ostream& os, const LogEntry& entry)
{
    os<<entry.date<<" "<<entry.time<<": "<<entry.path<<" "<<entry.message;
    return os;
}