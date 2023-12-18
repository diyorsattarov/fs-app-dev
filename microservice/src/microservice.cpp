#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/config.hpp>
#include <iostream>
#include <thread>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void handle_get(const std::string& target, http::response<http::string_body>& res) {
    if (target == "/") {
        spdlog::info("Microservice / route sent");
        res.body() = "Hello from C++ microservice";
    } else if (target == "/hello") {
        spdlog::info("Microservice /hello route sent");
        res.body() = "Hello, world!2";
    } else {
        res.result(http::status::not_found);
        res.body() = "Not Found3";
    }
}

void handle_post(const std::string& target, http::request<http::string_body>& req, http::response<http::string_body>& res) {
    if (target == "/data") {
        // Log the data received in the POST request
        spdlog::info("Received POST request at /data with body: {}", req.body());

        // Process POST data here
        res.body() = "Data received";
    } else {
        res.result(http::status::not_found);
        res.body() = "Not Found";
    }
}

void handle_request(http::request<http::string_body>&& req, http::response<http::string_body>& res) {
    // Set common headers
    res.set(http::field::server, BOOST_BEAST_DEPRECATION_STRING);
    res.set(http::field::content_type, "text/plain");

    // Dispatch based on method and path
    if (req.method() == http::verb::get) {
        handle_get(req.target().to_string(), res);
    } else if (req.method() == http::verb::post) {
        handle_post(req.target().to_string(), req, res); // pass req to handle_post
    } else {
        // Handle other HTTP methods or return a method not allowed error
        res.result(http::status::method_not_allowed);
        res.body() = "Method Not Allowed";
    }

    res.prepare_payload();  // Ensure the payload is prepared correctly
}

void do_session(tcp::socket& socket) {
    beast::flat_buffer buffer;
    http::request<http::string_body> req;

    http::read(socket, buffer, req);

    http::response<http::string_body> res{http::status::ok, req.version()};
    handle_request(std::move(req), res);

    http::write(socket, res);
}
int main() {
    try {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/microservice.log", true);
        std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};

        auto combined_logger = std::make_shared<spdlog::logger>("combined", begin(sinks), end(sinks));
        spdlog::register_logger(combined_logger);
        spdlog::set_default_logger(combined_logger);
        spdlog::set_level(spdlog::level::info); // Set global log level to info

        // Example of logging
        spdlog::info("Microservice starting...");
        auto const address = net::ip::make_address("0.0.0.0");
        unsigned short port = static_cast<unsigned short>(8080);

        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {address, port}};
        tcp::socket socket{ioc};

        while (true) {
            acceptor.accept(socket);
            std::thread{std::bind(
                &do_session, std::move(socket)
            )}.detach();
        }
    } catch (const std::exception& e) {
        spdlog::error("Exception: {}", e.what());
        return EXIT_FAILURE;
    }

    spdlog::info("Microservice exiting...");
    return EXIT_SUCCESS;
}
