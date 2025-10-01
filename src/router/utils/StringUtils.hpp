#pragma once

#include <string>
#include <string_view>

struct Location;

namespace router {
namespace utils {

class StringUtils {
    public:
        static std::string replaceAll(std::string str, const std::string& from, const std::string& to);
        static std::string replacePlaceholder(std::string html, const std::string& placeholder, const std::string& replacement);
        static std::string normalizePath(std::string path);
        static std::string determineFilePathCGI(const std::string_view& path, const Location* location, const std::string& server_root);
        static std::string resolvePath(const std::string& path, const std::string& server_root);

    private:
        StringUtils() = delete;
};

} // namespace utils
} // namespace router
