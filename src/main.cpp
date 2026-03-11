#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>

#include "rate_limiter.h"

void simulateClient(RateLimiter& limiter, const std::string& clientId, int requests) {
    for (int i = 0; i < requests; ++i) {
        bool allowed = limiter.allowRequest(clientId);
        std::cout << "Client " << clientId << " request " << (i + 1)
                  << (allowed ? " allowed" : " blocked") << '\n';

        // Small sleep to spread requests within/over windows
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    // Example: 10 requests per 1 second window
    RateLimiter limiter(10, std::chrono::seconds(1));

    const int clientCount = 5;
    const int requestsPerClient = 20;

    std::vector<std::thread> threads;
    threads.reserve(clientCount);

    for (int i = 0; i < clientCount; ++i) {
        std::string clientId = "client-" + std::to_string(i + 1);
        threads.emplace_back(simulateClient, std::ref(limiter), clientId, requestsPerClient);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Test complete." << std::endl;
    return 0;
}