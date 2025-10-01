# Webserver - HTTP Router Documentation

A high-performance HTTP/1.1 web server built in C++ with comprehensive routing capabilities, CGI support, file upload handling, and multi-server configuration management

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

## Installation Instructions

### 1. Clone and Build

Clone the repository and build the webserver:

```bash
git clone <repository-url>
cd webserv2
make run
```

## Usage Examples

Start the webserver with a configuration file:

```bash
./webserv configs/default.conf
```

### Multi-Server Configuration

Run multiple servers simultaneously:

```bash
# Server 1 - Full functionality (port 8080)
./webserv configs/default.conf

# Server 2 - GET/DELETE only (port 8081)
# Server 3 - GET/POST only (port 8082)
# Server 4 - Simple site (port 8083)
# Server 5 - Simple site (port 8084)
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
```

## Project Structure (Router Part)

[The project is organized into logical modules with clear separation of concerns:]

``` bash
webserver/
├── src/                          # Source code directory
│   ├── router/                   # HTTP routing system
│   │   ├── handlers/            # Request handlers (GET, POST, DELETE, CGI, redirect)
│   │   │   ├── Handlers.cpp     # Handler implementations
│   │   │   ├── CgiExecutor.cpp  # CGI script execution
│   │   │   ├── MultipartParser.cpp # File upload parsing
│   │   │   └── HandlerUtils.cpp # Handler utility functions
│   │   ├── utils/               # Router utility classes
│   │   │   ├── HttpResponseBuilder.cpp # HTTP response construction
│   │   │   ├── FileUtils.cpp    # File system operations
│   │   │   ├── StringUtils.cpp  # String manipulation utilities
│   │   │   ├── ValidationUtils.cpp # Input validation
│   │   │   └── Utils.cpp        # General utilities
│   │   ├── Router.cpp           # Main router class
│   │   ├── RequestProcessor.cpp # Request processing logic
│   │   ├── HttpConstants.hpp    # HTTP protocol constants
│   │   └── ROUTER_DOCS.md       # This documentation
│   ├── request/                 # HTTP request parsing
│   ├── response/                # HTTP response handling
│   ├── server/                  # Server configuration and management
│   └── main.cpp                 # Application entry point
├── configs/                     # Server configuration files
│   ├── default.conf            # Default server configuration
│   ├── extended.conf           # Extended multi-server configuration
│   └── repeating_IP_port.conf  # Port conflict testing configuration
├── test/                       # Test suite
│   ├── router_tests/           # Router-specific tests
│   │   ├── TESTS.md           # Comprehensive test documentation
│   │   └── run_all_tests.sh   # Automated test runner
│   └── [other test directories]
├── www/                        # Web content directory
│   ├── uploads/               # File upload destination
│   ├── errors/                # Custom error pages
│   ├── imgs/                  # Static images
│   ├── cgi-bin/               # CGI scripts
│   └── index.html             # Default index page
├── CMakeLists.txt             # CMake build configuration
├── Makefile                   # Alternative build system
└── README.md                  # Project overview
```

### Key Files and Directories

- **src/router/**: Core routing system with request handlers and utilities
- **src/router/handlers/**: HTTP method handlers (GET, POST, DELETE, CGI, redirect)
- **src/router/utils/**: Utility classes for HTTP response building, file operations, and validation
- **configs/**: Server configuration files with different setups for testing
- **test/router_tests/**: Comprehensive test suite with automated testing scripts
- **www/**: Web content directory structure with uploads, CGI scripts, and static files

## Router Architecture

### Core Components

- **Router**: Main routing class that manages route mappings and handler selection
- **RequestProcessor**: Handles request execution and fallback logic
- **Handlers**: Specific implementations for different request types:
  - `get()`: Static file serving and directory listing
  - `post()`: File upload handling via multipart/form-data
  - `del()`: File deletion from upload directories
  - `cgi()`: CGI script execution with environment setup
  - `redirect()`: HTTP redirection handling

### Route Storage Structure

```cpp
std::map<int, std::map<std::string, std::map<std::string, Handler>>> _routes;
// server_id → HTTP_method → path → Handler_function
```

### Handler Selection Logic

The router automatically selects the appropriate handler based on location configuration and HTTP method:

```cpp
// Selection Priority
if (!location.return_url.empty()) {
    // Use redirect handler
} else if (!location.cgi_path.empty() && !location.cgi_ext.empty()) {
    // Use CGI handler
} else if (method == http::POST && !location.upload_path.empty()) {
    // Use POST handler for uploads
} else if (method == http::DELETE && !location.upload_path.empty()) {
    // Use DELETE handler for file removal
} else {
    // Use GET handler as default
}
```

### Route Resolution Priority

The `findHandler()` method implements sophisticated route matching:

1. **Exact Match**: Highest priority for precise path matches
2. **Extension Match**: Matches file extensions (e.g., `.py`, `.php`)
3. **Prefix Match**: Matches directory prefixes (e.g., `/uploads`, `/admin`)

## Testing

Comprehensive testing approach with automated test suite:

### Running Tests

Execute the complete test suite:

```bash
# Make sure webserver is running on ports 8080-8084
./webserv configs/extended.conf
```

```bash
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

**Webserver** - A high-performance HTTP/1.1 web server with advanced routing capabilities

*Built with modern C++20, featuring comprehensive HTTP method support, CGI execution, file upload handling, and multi-server configuration management.*
