#pragma once

#include <string>
#include "../HttpConstants.hpp"

// Forward declarations
class Response;
class Request;
class Server;

namespace router {
namespace utils {

class HttpResponseBuilder {
  public:
    static void setErrorResponse(Response& res, int status, const class Request& req);static void setErrorResponse(Response& res, int status, const class Request& req, const class Server& server);
    static void setMethodNotAllowedResponse(Response& res, const std::vector<std::string>& allowedMethods, const class Request& req);
    static void setSuccessResponse(Response& res, const std::string& content, const std::string& contentType, const class Request& req);
    static std::string getErrorPageHtml(int status);
    static std::string getErrorPageHtml(int status, const class Server& server);
    static std::string makeDefaultErrorPage(int code, const std::string& reason);
    static std::string makeDefaultSuccessPage(int code, const std::string& reason);
    static void setSuccessResponseWithDefaultPage(Response& res, int status, const class Request& req);
    static std::string getSuccessPageHtml(int status);
    static int parseStatusCodeFromString(const std::string& statusString);

  private:
    HttpResponseBuilder() = delete;
};

} // namespace utils
} // namespace router
