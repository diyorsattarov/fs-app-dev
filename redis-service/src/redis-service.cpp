#include <sw/redis++/redis++.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <vector>

void initialize_logging() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/redis-service.log", true);
    std::vector<spdlog::sink_ptr> sinks = {console_sink, file_sink};

    auto combined_logger = std::make_shared<spdlog::logger>("combined", sinks.begin(), sinks.end());
    spdlog::register_logger(combined_logger);
    spdlog::set_default_logger(combined_logger);
    spdlog::set_level(spdlog::level::info);
}

int main(int argc, char* argv[]) {
    initialize_logging();
    spdlog::info("Redis service starting...");

    try {
        auto redis = sw::redis::Redis("tcp://redis:6379");
        if (argc < 2) {
            spdlog::error("Usage: {} [SET/GET] [key] [value (for SET)]", argv[0]);
            return 1;
        }

        std::string command = argv[1];
        if (command == "SET" && argc == 4) {
            std::string key = argv[2];
            std::string value = argv[3];
            redis.set(key, value);
            spdlog::info("Set key '{}' with value '{}'", key, value);
        } else if (command == "GET" && argc == 3) {
            std::string key = argv[2];
            auto val = redis.get(key);
            if (val) {
                spdlog::info("Value for key '{}': {}", key, *val);
            } else {
                spdlog::info("Key '{}' does not exist", key);
            }
        } else {
            spdlog::error("Invalid command or arguments");
            return 1;
        }
    } catch (const std::exception& e) {
        spdlog::error("Exception: {}", e.what());
        return EXIT_FAILURE;
    }

    spdlog::info("Redis service exiting...");
    return EXIT_SUCCESS;
}
