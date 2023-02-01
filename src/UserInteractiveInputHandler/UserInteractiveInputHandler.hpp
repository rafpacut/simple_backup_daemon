#ifndef USER_INTERACTIVE_INPUT_HANDLER
#define USER_INTERACTIVE_INPUT_HANDLER

#include"../AppLogReader/AppLogReader.hpp"
#include"../LogPresenter/LogPresenter.hpp"
#include"../InputParser/InputParser.hpp"


class UserInteractiveInputHandler
{
    public:
        UserInteractiveInputHandler(const InputParser& ip)
        : ip(ip)
        , log_reader()
        , presenter() {}

        void handle_log_display_queries(const fs::path& log_file_path) const;

    private:
        const InputParser& ip;
        const AppLogReader log_reader;
        const LogPresenter presenter;
        const std::string prompt = 
"\n**********************************************************************************\n\
* To view unfiltered log press enter                                             *\n\
* To view filtered log provide filters: <date_regex> <filename_regex> examples:  *\n\
* .* .*                                                                          *\n\
* .* [a-z]+[0-9]+.txt                                                            *\n\
* 2023-[01|02].* .*                                                              *\n\
* .* .*.[txt|mp3]                                                                *\n\
* To terminate the program type 'q'                                              *\n\
**********************************************************************************\n";
};
#endif