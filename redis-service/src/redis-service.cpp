#include <sw/redis++/redis++.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        // Initialize Redis client
        auto redis = sw::redis::Redis("tcp://redis:6379");

        // Check if enough arguments are passed (program name, command, key, and optionally value)
        if (argc < 3) {
            std::cerr << "Usage: " << argv[0] << " [SET key value] or [GET key]" << std::endl;
            return 1;
        }

        std::string command = argv[1];
        std::string key = argv[2];

        if (command == "SET") {
            if (argc != 4) {
                std::cerr << "SET command requires a key and a value." << std::endl;
                return 1;
            }
            std::string value = argv[3];
            redis.set(key, value);
            std::cout << "OK" << std::endl;

        } else if (command == "GET") {
            auto val = redis.get(key);
            if (val) {
                std::cout << *val << std::endl;
            } else {
                std::cout << "(nil)" << std::endl;
            }

        } else {
            std::cerr << "Unsupported command: " << command << std::endl;
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "Redis++ exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
