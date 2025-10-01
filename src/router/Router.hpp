/**
 * @file Router.hpp
 * @brief HTTP Router for request routing and handler management
 */

#pragma once

#include <map>
#include <string>
#include <string_view>
#include <functional>

#include "../request/Request.hpp"
#include "../response/Response.hpp"
#include "../server/Server.hpp"
#include "HttpConstants.hpp"
#include "RequestProcessor.hpp"

/**
 * @class Router
 * @brief Manages HTTP route mappings and request routing
 */
class Router {
public:
  Router();
  ~Router();

  using Handler = std::function<void(const Request&, Response&, const Server&)>;

  void setupRouter(const std::vector<Server>& configs);
  void handleRequest(const Server& server, const Request& req, Response& res) const;
  void listRoutes() const;

private:
  void addRoute(int server_id, std::string_view method, std::string_view path, Handler handler);
  const Handler* findHandler(int server_id, const std::string& method, const std::string& path) const;
  const Location* findLocation(const Server& server, const std::string& path) const;

  std::map<int, std::map<std::string, std::map<std::string, Handler>>> _routes;
  RequestProcessor _requestProcessor;
};
