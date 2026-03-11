#pragma once

#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>

// Thread-safe fixed window rate limiter.
// Usage:
//   RateLimiter limiter(100, std::chrono::seconds(1));
//   if (limiter.allow("client-id")) { ... }
class RateLimiter {
public:
    using Clock = std::chrono::steady_clock;

    // max_requests: maximum number of allowed requests per window
    // window_duration: duration of a single fixed window
    RateLimiter(std::size_t max_requests,
                std::chrono::milliseconds window_duration);

    // Returns true if the request from `client_id` is allowed, false otherwise.
    bool allowRequest(const std::string& client_id);

    // Optional helpers
    std::size_t maxRequests() const noexcept { return max_requests_; }
    std::chrono::milliseconds windowDuration() const noexcept { return window_duration_; }

private:
    struct ClientWindow {
        Clock::time_point window_start;
        std::size_t count;
    };

    const std::size_t max_requests_;
    const std::chrono::milliseconds window_duration_;

    mutable std::mutex mutex_;
    std::unordered_map<std::string, ClientWindow> clients_;
};

