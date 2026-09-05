#include <filesystem>
#include <string_view>

void process_dir(
    const std::filesystem::path& dirPath,
    std::string_view action
);