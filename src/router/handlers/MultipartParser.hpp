/**
 * @file MultipartParser.hpp
 * @brief Multipart form data parser
 */

#pragma once

#include <string>
#include <vector>

namespace router::handlers {

struct MultipartData {
  std::string filename;
  std::string content;
  bool isValid = false;
};

class MultipartParser {
  public:
    static MultipartData parseMultipartData(const std::string& body, const std::string& boundary);

  private:
    static std::string extractFilename(const std::string& filePart);
    static std::string extractFileContent(const std::string& filePart);
    static std::string cleanFileContent(const std::string& content);
};

} // namespace router::handlers
