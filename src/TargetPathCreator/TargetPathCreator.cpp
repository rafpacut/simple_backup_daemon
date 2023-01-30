#include"TargetPathCreator.hpp"

fs::path TargetPathCreator::create_target_path_for_tagged_entry(const fs::directory_entry& entry) const
{
    fs::path path_no_tag = create_target_path_without_tag(entry.path());
    if(entry.is_regular_file())
    {
        return create_target_path_for_regular_file(path_no_tag);
    }
    if(entry.is_directory())
    {
        return create_target_path(path_no_tag);
    }
    if(entry.is_symlink())
    {
        return create_target_path_for_symlink(path_no_tag, entry.status());
    }
    return fs::path{};
}

fs::path TargetPathCreator::create_target_path_for_symlink(const fs::path& path, const fs::file_status& link_target_file_status) const
{
    if(fs::is_regular_file(link_target_file_status))
    {
        return create_target_path_for_regular_file(path);
    }
    if(fs::is_directory(link_target_file_status))
    {
        return create_target_path(path);
    }
    //it'll try and copy weird files
    return fs::path{};
}

fs::path TargetPathCreator::create_target_path_for_regular_file(const fs::path& path) const
{
    fs::path target_path = create_target_path(path);
    target_path.replace_extension(target_path.extension().string() + ".bak");
    return target_path;
}

//assumes filepath and src_base_path have sth in common
fs::path TargetPathCreator::create_target_path(const fs::path& path) const
{
    const auto relative_path = create_path_relative_to_source_base_path(path);
    const auto fullpath_str = target_base_path.string() + relative_path.string();
    return fs::path(fullpath_str);
}

fs::path TargetPathCreator::create_path_relative_to_source_base_path(const fs::path& path) const
{
   const auto base_src_path_length = src_base_path.string().length();
   const auto filepath_str = path.string();
   const auto path_relative_to_source_base_path = std::string(filepath_str.begin()+base_src_path_length,
                                                                filepath_str.end());
   return fs::path(path_relative_to_source_base_path);
}

fs::path TargetPathCreator::create_target_path_without_tag(const fs::path& path) const
{
    fs::path new_path = path;
    const auto filename_without_tag = get_filename_without_tag(path.filename().string());
    new_path.replace_filename(filename_without_tag);

    return new_path;
}

std::string TargetPathCreator::get_filename_without_tag(const std::string& filename_str) const
{
    const auto filename_without_prefix = std::string{filename_str.begin()+DELETE_TAG_LENGTH, filename_str.end()};
    return filename_without_prefix;
}