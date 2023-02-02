#include<chrono>
#include<thread>
#include"BackupDaemon.hpp"
#include"../utils/utils.hpp"

bool BackupDaemon::is_tagged_for_removal(const fs::directory_entry& entry) const
{
    const auto filename_str = entry.path().filename().string();
    if( filename_str.size() <= DELETE_PREFIX_LENGTH)
    {
        return false;
    }
    const auto filename_prefix = 
        std::string(filename_str.begin(), filename_str.begin()+DELETE_PREFIX_LENGTH);
    return filename_prefix == DELETE_PREFIX;
}

void BackupDaemon::remove_src_file_without_tag(const fs::path& tag_file_path) const
{
    auto filename_str = tag_file_path.filename().string();
    if(filename_str.size() < DELETE_PREFIX_LENGTH)
    {
        return;
    }

    const auto filename_str_no_tag = std::string(filename_str.begin()+DELETE_PREFIX_LENGTH, filename_str.end());
    auto source_path_no_tag = tag_file_path;
    source_path_no_tag.replace_filename(filename_str_no_tag);
    if(fs::exists(source_path_no_tag))
    {
        fs_op_wrap.remove(source_path_no_tag);
    }
}

void BackupDaemon::resume_interrupted_copies() const
{
    for(const auto entry : fs::recursive_directory_iterator(target_base_path))
    {
        if(utils::is_tmp_file(entry))
        {
            const auto path_no_tmp_ext = utils::strip_tmp_ext(entry.path());
            const auto path_no_bak_ext = utils::strip_bak_ext(path_no_tmp_ext);
            const auto path_on_source = tpc.create_source_path_for_target_path(path_no_bak_ext);
            if(fs::exists(path_on_source))
            {
                const auto target_path_no_tmp = utils::strip_tmp_ext(entry.path());
                fs_op_wrap.remove(entry.path());
                fs_op_wrap.copy_file(path_on_source, target_path_no_tmp);
            }
        }
    }
}

void BackupDaemon::backup_entry(const fs::directory_entry& entry)
{
    fs::path target_path;
    if(is_tagged_for_removal(entry))
    {
        target_path = tpc.create_target_path_for_tagged_entry(entry);
        remove_src_file_without_tag(entry.path());
        fs_op_wrap.remove(entry.path());
        fs_op_wrap.remove(target_path);
    }
    if(entry.is_directory())
    {
        target_path = tpc.create_target_path(entry.path());
        fs_op_wrap.create_directory(target_path);
    }
    if(entry.is_symlink())
    {
        try
        {
            const fs::path symlink_path = fs::read_symlink(entry.path());
            target_path = tpc.create_target_path_for_symlink(entry.path(), entry.status());
            fs_op_wrap.copy_file(symlink_path, target_path);
        }
        catch(fs::filesystem_error& e)
        {
            logger.log_error(entry.path(), e.what());
            return;
        }
    }
    if(entry.is_regular_file())
    {
        target_path = tpc.create_target_path_for_regular_file(entry.path());
        fs_op_wrap.copy_file(entry.path(), target_path);
    }
    path_cache.insert({entry.path().string(), target_path.string()});
}

bool contains(const std::unordered_map<std::string,std::string>& map, const std::string& key)
{
    return map.find(key) != map.end();
}

void BackupDaemon::operator()(std::atomic_bool& running)
{
    resume_interrupted_copies();
    
    while(running.load())
    {
        std::this_thread::sleep_for(DELAY);
        for(const auto entry : fs::recursive_directory_iterator(src_base_path, fs::directory_options::follow_directory_symlink))
        {
            if(not running.load())
            {
                return;
            }

            const auto path_was_cached = contains(path_cache, entry.path().string());
            if(not path_was_cached)
            {
                backup_entry(entry);
            }
            else
            {
                fs::path target_path = path_cache.at(entry.path().string());
                if(path_was_cached and fs::exists(target_path) and utils::is_file_newer(entry.path(), target_path))
                {
                    backup_entry(entry);
                }
            }
        }
    }
}