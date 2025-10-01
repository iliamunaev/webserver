/**
 * @file Handlers.hpp
 * @brief HTTP request handler functions
 */

#pragma once

#include <string>
#include <vector>

#include "../../request/Request.hpp"
#include "../../response/Response.hpp"

struct Location;
class Server;

void get(const Request& req, Response& res, const Server& server);
void post(const Request& req, Response& res, const Server& server);
void del(const Request& req, Response& res, const Server& server);
void cgi(const Request& req, Response& res, const Server& server);
void redirect(const Request& req, Response& res, const Server& server);
