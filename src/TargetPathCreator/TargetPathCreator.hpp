#ifndef TARGET_PATH_CREATOR
#define TARGET_PATH_CREATOR

#include<filesystem>
namespace fs = std::filesystem;

class TargetPathCreator
{
    public:
        TargetPathCreator(const fs::path& src_base_path, const fs::path& target_base_path) : src_base_path(src_base_path), target_base_path(target_base_path) {}

        fs::path create_target_path_for_tagged_entry(const fs::directory_entry&) const;
        fs::path create_target_path_for_regular_file(const fs::path&) const;
        fs::path create_target_path_for_symlink(const fs::path&, const fs::file_status&) const;
        fs::path create_target_path(const fs::path&) const;

    private:
        fs::path create_path_relative_to_source_base_path(const fs::path&) const;

        fs::path create_target_path_without_tag(const fs::path&) const;
        std::string get_filename_without_tag(const std::string&) const;

        const fs::path& src_base_path;
        const fs::path& target_base_path;
        const std::string DELETE_TAG = "delete_";
        const int DELETE_TAG_LENGTH = DELETE_TAG.size();
        const std::string BACKUP_SUFFIX = ".bak";
        const int BACKUP_SUFFIX_LENGTH = BACKUP_SUFFIX.size();
};


#endif