# Webserver - HTTP/1.1 Web Server Implementation

A high-performance HTTP/1.1 web server built in C++20 with comprehensive routing capabilities, CGI support, file upload handling, and multi-server configuration management.

## Features

Webserver is a full-featured HTTP server implementation with advanced routing capabilities:

- **Multi-Server Support**: Run multiple virtual servers on different ports with independent configurations
- **HTTP Method Routing**: Complete support for GET, POST, DELETE methods with method-specific handlers
- **CGI Execution**: RFC 3875 compliant CGI script execution with environment setup and timeout handling
- **File Upload System**: Multipart/form-data file upload handling with size validation and directory management
- **Autoindex Directory Listing**: Automatic HTML directory listing with navigation support
- **HTTP Redirection**: Configurable URL redirection with proper status codes
- **Advanced Error Handling**: Comprehensive HTTP error response system with custom error pages

### Core HTTP Processing

- **Request Parsing**: Complete HTTP/1.1 request parsing with header validation
- **Response Building**: Dynamic HTTP response generation with proper headers and status codes
- **Route Matching**: Sophisticated path matching with exact, extension, and prefix matching strategies
- **Method Validation**: HTTP method validation with proper 405 Method Not Allowed responses
- **Content Type Detection**: Automatic MIME type detection and Content-Type header setting

### Advanced Features

- **Chunked Transfer Encoding**: Support for chunked request and response bodies
- **Connection Management**: Keep-alive and close connection handling
- **File Size Validation**: Configurable client_max_body_size with 413 Payload Too Large responses
- **CGI Environment Setup**: Complete CGI environment variable configuration
- **Nested Directory Support**: Deep directory structure navigation with autoindex

## Requirements

## Installation Instructions

### 1. Clone and Build

Clone the repository and build the webserver:

```bash
git clone <repository-url>
cd webserv
make run
```

### 2. Usage Examples

### Basic Server Startup

Start the webserver with a configuration file:

```bash
./webserv configs/default.conf
```

### Multi-Server Configuration

Run multiple servers simultaneously:

```bash
# Server 1 - Full functionality (port 8080)
./webserv configs/extended.conf

# Multiple servers will be available on:
# - http://127.0.0.1:8080 (full features)
# - http://127.0.0.1:8081 (read-only)
# - http://127.0.0.1:8082 (upload-only)
# - http://127.0.0.1:8083 (site2)
# - http://127.0.0.1:8084 (site3)
```

### Static File Serving

Access static files and directories:

```bash
# Root directory with index.html
curl http://127.0.0.1:8080/

# Specific static files
curl http://127.0.0.1:8080/index.html
curl http://127.0.0.1:8080/favicon.ico

# Directory listing with autoindex
curl http://127.0.0.1:8080/uploads/
curl http://127.0.0.1:8080/imgs/
```

### File Upload Operations

Upload files using POST method:

```bash
# Create a test file
echo "Hello, Webserver!" > test.txt

# Upload the file
curl -X POST -F "file=@test.txt" http://127.0.0.1:8080/uploads/

# Verify upload
curl http://127.0.0.1:8080/uploads/test.txt
```

### File Deletion

Remove uploaded files:

```bash
# Delete an uploaded file
curl -X DELETE http://127.0.0.1:8080/uploads/test.txt

# Verify deletion (should return 404)
curl http://127.0.0.1:8080/uploads/test.txt
```

### CGI Script Execution

Execute CGI scripts with different methods:

```bash
# GET request to CGI script
curl http://127.0.0.1:8080/cgi-bin/hello.py

# POST with form data
curl -X POST -d "name=test&value=123" http://127.0.0.1:8080/cgi-bin/hello.py

# POST with JSON data
curl -X POST -H "Content-Type: application/json" \
     -d '{"name":"test","value":123}' \
     http://127.0.0.1:8080/cgi-bin/hello.py
```

### HTTP Redirection

Test URL redirections:

```bash
# Follow redirect (302 Found)
curl -L http://127.0.0.1:8080/old

# Check redirect without following
curl -v http://127.0.0.1:8080/old
```

### Error Handling Examples

Test various error conditions:

```bash
# 404 Not Found
curl -v http://127.0.0.1:8080/nonexistent.html

# 405 Method Not Allowed
curl -v -X PUT http://127.0.0.1:8080/
curl -v -X PATCH http://127.0.0.1:8080/uploads/

# 413 Payload Too Large
dd if=/dev/zero of=large.txt bs=1K count=2000
curl -X POST -F "file=@large.txt" http://127.0.0.1:8080/uploads/
```

### Complete Workflow Test

Test a complete upload → verify → delete workflow:

```bash
# 1. Upload a file
echo "workflow test $(date)" > workflow_test.txt
curl -X POST -F "file=@workflow_test.txt" http://127.0.0.1:8080/uploads/

# 2. Verify the file exists
curl http://127.0.0.1:8080/uploads/workflow_test.txt

# 3. Check directory listing
curl http://127.0.0.1:8080/uploads/

# 4. Delete the file
curl -X DELETE http://127.0.0.1:8080/uploads/workflow_test.txt

# 5. Verify deletion
curl http://127.0.0.1:8080/uploads/workflow_test.txt  # Should return 404
```## Project Architecture
```

### Core Components

```bash
webserv/
├── src/                          # Source code directory
│   ├── main.cpp                  # Application entry point
│   ├── config/                   # Configuration management
│   │   ├── Config.cpp           # Main configuration class
│   │   ├── ConfigExtractor.cpp  # Configuration file parsing
│   │   └── ConfigValidator.cpp  # Configuration validation
│   ├── server/                   # Server management
│   │   ├── Cluster.cpp          # Multi-server cluster management
│   │   ├── Server.cpp           # Individual server implementation
│   │   └── HelperFunctions.cpp  # Server utility functions
│   ├── router/                   # HTTP routing system
│   │   ├── Router.cpp           # Main router class
│   │   ├── RequestProcessor.cpp # Request processing logic
│   │   ├── handlers/            # Request handlers
│   │   │   ├── Handlers.cpp     # Handler implementations
│   │   │   ├── CgiExecutor.cpp  # CGI script execution
│   │   │   ├── MultipartParser.cpp # File upload parsing
│   │   │   └── HandlerUtils.cpp # Handler utility functions
│   │   └── utils/               # Router utility classes
│   │       ├── HttpResponseBuilder.cpp # HTTP response construction
│   │       ├── FileUtils.cpp    # File system operations
│   │       ├── StringUtils.cpp  # String manipulation utilities
│   │       ├── ValidationUtils.cpp # Input validation
│   │       └── Utils.cpp        # General utilities
│   ├── request/                  # HTTP request handling
│   │   ├── Request.cpp          # HTTP request parsing and management
│   │   └── Request.hpp          # Request class definition
│   ├── response/                 # HTTP response handling
│   │   ├── Response.cpp         # HTTP response building and management
│   │   └── Response.hpp         # Response class definition
│   ├── message/                  # Message base classes
│   │   └── AMessage.cpp         # Abstract message base class
│   └── parser/                   # HTTP parsing utilities
│       ├── Parser.cpp           # HTTP message parsing
│       └── ParserUtils.cpp      # Parsing utility functions
├── inc/                          # Header files
│   └── webserv.hpp              # Main header with constants and definitions
├── configs/                      # Server configuration files
│   ├── default.conf             # Default server configuration
│   ├── extended.conf            # Extended multi-server configuration
│   └── repeating_IP_port.conf   # Port conflict testing configuration
├── test/                         # Test suite
│   ├── router_tests/            # Router-specific tests
│   │   ├── TESTS.md            # Comprehensive test documentation
│   │   └── run_all_tests.sh    # Automated test runner
│   └── end-to-end/              # End-to-end integration tests
├── www/                          # Web content directory
│   ├── webserv_project/         # Main web content
│   │   ├── uploads/            # File upload destination
│   │   ├── errors/             # Custom error pages
│   │   ├── imgs/               # Static images
│   │   ├── cgi-bin/            # CGI scripts
│   │   ├── nested/             # Nested directory structure for testing
│   │   └── index.html          # Default index page
│   ├── site2/                   # Alternative site content
│   └── site3/                   # Alternative site content
├── Makefile                     # Build system
└── README.md                    # This documentation
```

### Key Architectural Components

#### 1. **Cluster Management**

- **Cluster Class**: Manages multiple server instances and client connections
- **Server Class**: Individual server configuration and socket management
- **Listener Groups**: Groups servers with same IP/port combinations

#### 2. **HTTP Processing Pipeline**

- **Request Parser**: Parses incoming HTTP requests
- **Router**: Routes requests to appropriate handlers
- **Response Builder**: Constructs HTTP responses
- **Message Classes**: Base classes for Request/Response

#### 3. **Router System**

- **Route Storage**: `std::map<server_id, std::map<method, std::map<path, handler>>>`
- **Handler Types**: GET, POST, DELETE, CGI, Redirect handlers
- **Route Matching**: Exact, extension, and prefix matching strategies

#### 4. **Configuration System**

- **Config Parser**: Parses configuration files
- **Config Validator**: Validates configuration syntax
- **Config Extractor**: Extracts server and location configurations

## Testing

### Running Tests

Execute the complete test suite:

```bash
# Start the webserver with extended configuration
./webserv configs/extended.conf
```

```bash
# Run comprehensive router tests
./test/router_tests/run_all_tests.sh
```

### Unit Testing

Run the comprehensive test suite:

```bash
# Run automated router tests
./test/router_tests/run_all_tests.sh
```

### Test Coverage

The test suite covers:

- **Basic HTTP Operations**: GET, POST, DELETE methods
- **Static File Serving**: File serving, directory listing, MIME types
- **File Upload/Download**: Multipart uploads, file verification, deletion
- **CGI Execution**: Script execution with different input methods
- **HTTP Redirection**: Redirect handling and status codes
- **Error Handling**: 404, 405, 413, 500, 504 error responses
- **Multi-Server Support**: Multiple server configurations
- **Nested Directory Support**: Deep directory structure navigation

### Manual Testing

Individual test categories can be run manually:

```bash
# Basic GET tests
curl -v http://127.0.0.1:8080/

# POST upload tests
curl -X POST -F "file=@test.txt" http://127.0.0.1:8080/uploads/

# DELETE tests
curl -X DELETE http://127.0.0.1:8080/uploads/test.txt

# CGI tests
curl http://127.0.0.1:8080/cgi-bin/hello.py
```

## Configuration

### Configuration File Format

The webserver uses a custom configuration format similar to nginx:

```nginx
server {
    server_name webserv
    listen 8080
    host 127.0.0.1
    root www/webserv_project
    index index.html
    client_max_body_size 10000000
    error_page 404 errors/not_found_404.html

    location / {
        allow_methods GET
        autoindex on
        index index.html
    }

    location /uploads {
        allow_methods GET DELETE POST
        upload_to uploads
        autoindex on
    }

    location /cgi-bin {
        allow_methods GET POST
        cgi_path cgi-bin
        cgi_ext .py .js
    }
}
```

### Configuration Directives

- **server**: Defines a server block
- **server_name**: Server identifier
- **listen**: Port number to listen on
- **host**: IP address to bind to
- **root**: Document root directory
- **index**: Default index file
- **client_max_body_size**: Maximum request body size
- **error_page**: Custom error page mapping
- **location**: URL path configuration
- **allow_methods**: Allowed HTTP methods
- **autoindex**: Enable directory listing
- **upload_to**: File upload destination
- **cgi_path**: CGI script directory
- **cgi_ext**: CGI file extensions
- **return**: Redirect URL

## Development

### Build Options

#### Clean Build

```bash
make clean # or make fclean
make
```

#### Parallel Build

```bash
make -j6
```

### Code Style

- **C++ Standard**: C++20
- **Indentation**: 2 spaces
- **Class Structure**: Public members before private members
- **Modern C++**: Use RAII, standard library containers, and lambda functions
- **Memory Management**: Stack allocation preferred, explicit cleanup for system resources

## API Documentation

### Router Documentation

For detailed information about the router system, see:

- [**Router Documentation**](src/router/ROUTER_DOCS.md)

### Core Classes

#### Cluster

- **Purpose**: Manages multiple server instances and client connections
- **Key Methods**: `config()`, `create()`, `run()`
- **Features**: Multi-server support, client connection management, timeout handling

#### Router

- **Purpose**: Routes HTTP requests to appropriate handlers
- **Key Methods**: `setupRouter()`, `handleRequest()`, `findHandler()`
- **Features**: Route matching, handler selection, request processing

#### Server

- **Purpose**: Individual server configuration and management
- **Key Methods**: `create()`, `setLocation()`, `getLocations()`
- **Features**: Socket management, configuration storage, location handling

#### Request/Response

- **Purpose**: HTTP message handling
- **Features**: Parsing, validation, header management, body handling

## Troubleshooting

### Common Issues

1. **Port Already in Use**

   ```bash
   # Check what's using the port
   sudo netstat -tlnp | grep :8080
   # Kill the process or use a different port
   ```

2. **Permission Denied**

   ```bash
   # Make sure CGI scripts are executable
   chmod +x www/cgi-bin/*.py
   chmod +x www/cgi-bin/*.js
   ```

3. **Configuration Errors**

   ```bash
   # Validate configuration syntax
   ./webserv configs/your_config.conf
   # Check for syntax errors in the output
   ```

4. **Build Issues**

   ```bash
   # Clean and rebuild
   make clean
   make
   ```

### Development Team

- **Developers**: [Ilia Munaev](https://www.linkedin.com/in/iliamunaev/), [Laurens Haas](https://github.com/LaurensH962), [Vladimir Lopatinski](https://github.com/vallucodes)

- **Organization**: 42 School (Hive Halsinki)

---

**Webserver** - A high-performance HTTP/1.1 web server with advanced routing capabilities

*Built with modern C++20, featuring comprehensive HTTP method support, CGI execution, file upload handling, and multi-server configuration management.*
