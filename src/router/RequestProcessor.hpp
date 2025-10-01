/**
 * @file RequestProcessor.hpp
 * @brief Request processing logic for HTTP request handling
 */

#pragma once

#include <string>
#include <functional>
#include "../request/Request.hpp"
#include "../response/Response.hpp"
#include "../server/Server.hpp"
#include "HttpConstants.hpp"

struct Location;
using Handler = std::function<void(const Request&, Response&, const Server&)>;

/**
 * @class RequestProcessor
 * @brief Processes HTTP requests and coordinates with handlers
 */
class RequestProcessor {
  public:
    RequestProcessor();
    ~RequestProcessor();
    void processRequest(const Request& req, const Handler* handler,
                        Response& res, const Server& server) const;

  private:
    bool executeHandler(const Handler* handler,
                          const Request& req, Response& res,
                          const Server& server) const;
    bool tryServeAsStaticFile(const Request& req, Response& res,
                               const std::string& method, const Server& server) const;
    bool isPathExistsButMethodNotAllowed(const Request& req, const Server& server) const;
    bool isPathConfigured(const Request& req, const Server& server) const;
    const Location* findLocationForPath(const Server& server, const std::string& path) const;
};
