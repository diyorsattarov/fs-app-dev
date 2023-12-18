#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
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

// Function to handle HTTP requests and send responses
void handle_request(tcp::socket& socket) {
    beast::flat_buffer buffer;
    http::request<http::string_body> req;
    http::read(socket, buffer, req);

    // Create a response
    http::response<http::string_body> res;
    const std::string target = req.target().to_string();

    if (req.method() == http::verb::get) {
        if (target == "/") {
            res.result(http::status::ok);
            res.body() = "Hello from C++ microservice";
        } else if (target == "/hello") {
            res.result(http::status::ok);
            res.body() = "Hello, from /hello!";
        } else {
            res.result(http::status::not_found);
            res.body() = "Not Found";
        }
    } else if (req.method() == http::verb::post) {
        if (target == "/data") {
            res.result(http::status::ok);
            res.body() = "Data received";
        } else {
            res.result(http::status::not_found);
            res.body() = "Not Found";
        }
    } else {
        res.result(http::status::method_not_allowed);
        res.body() = "Method Not Allowed";
    }

    res.set(http::field::server, BOOST_BEAST_DEPRECATION_STRING);
    res.set(http::field::content_type, "text/plain");
    res.prepare_payload();

    http::write(socket, res);
}

int main() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/microservice.log", true);
    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};

    auto combined_logger = std::make_shared<spdlog::logger>("combined", begin(sinks), end(sinks));
    spdlog::register_logger(combined_logger);
    spdlog::set_default_logger(combined_logger);
    spdlog::set_level(spdlog::level::info);

    spdlog::info("Microservice starting...");

    try {
        net::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {net::ip::make_address("0.0.0.0"), static_cast<unsigned short>(8080)}};
        spdlog::info("Server listening on port 8080");

        while (true) {
            tcp::socket socket{ioc};
            spdlog::info("Waiting for new connection...");
            acceptor.accept(socket);

            handle_request(socket);
        }
    } catch (const std::exception& e) {
        spdlog::error("Exception: {}", e.what());
        return EXIT_FAILURE;
    }

    spdlog::info("Microservice exiting...");
    return EXIT_SUCCESS;
}
