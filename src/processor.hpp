#include <filesystem>
#include <string_view>

void process_dir(
    std::filesystem::path dirPath,
    std::string_view action
);