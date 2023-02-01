#include"BackupDaemon.hpp"
#include<chrono>
#include<thread>

bool BackupDaemon::is_tagged_for_removal(const fs::directory_entry& entry) const
{
    const auto filename_str = entry.path().filename().string();
    if( filename_str.size() <= DELETE_PREFIX_LENGTH)
    {
        return false;
    }
    const auto filename_prefix = std::string(filename_str.begin(), filename_str.begin()+DELETE_PREFIX_LENGTH);
    return filename_prefix == DELETE_PREFIX;
}

void BackupDaemon::remove_src_file_without_tag(const fs::path& tag_file_path) const
{
    auto filename_str = tag_file_path.filename().string();
    const auto filename_str_no_tag = std::string(filename_str.begin()+DELETE_PREFIX_LENGTH, filename_str.end());

    auto source_path_no_tag = tag_file_path;
    source_path_no_tag.replace_filename(filename_str_no_tag);
    if(fs::exists(source_path_no_tag))
    {
        fs_op_wrap.remove(source_path_no_tag);
    }
}

void BackupDaemon::operator()(std::atomic_bool& running)
{
    while(running.load())
    {
        //200 constant buried in implementation...
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        for(const auto entry : fs::recursive_directory_iterator(src_base_path, fs::directory_options::follow_directory_symlink))
        {
            if(running.load())
            {
                break;
            }
            if(is_tagged_for_removal(entry))
            {
                const auto target_path = tpc.create_target_path_for_tagged_entry(entry);
                remove_src_file_without_tag(entry.path());
                fs_op_wrap.remove(entry.path());
                fs_op_wrap.remove(target_path);
            }
            else if(entry.is_directory())
            {
                const auto target_path = tpc.create_target_path(entry.path());
                fs_op_wrap.create_directory(target_path);
            }
            else if(entry.is_symlink())
            {
                //this could be confusing
                //why declare error_code if I'm not using it?
                //because then read_symlink returns empty path on error
                //which is fine by me
                std::error_code error_code;
                const fs::path symlink_path = fs::read_symlink(entry.path(), error_code);//returns fs::path{} on error
                const auto target_path = tpc.create_target_path_for_symlink(entry.path(), entry.status());
                fs_op_wrap.copy_file(symlink_path, target_path);
            }
            else if(entry.is_regular_file())
            {
                const auto target_path = tpc.create_target_path_for_regular_file(entry.path());
                fs_op_wrap.copy_file(entry.path(), target_path);
            }
        }
    }
}