#include "rate_limiter.h"

RateLimiter::RateLimiter(std::size_t max_requests,
                         std::chrono::milliseconds window_duration)
    : max_requests_(max_requests),
      window_duration_(window_duration) {}

bool RateLimiter::allowRequest(const std::string& client_id) {
    const auto now = Clock::now();

    std::lock_guard<std::mutex> lock(mutex_);

    auto& entry = clients_[client_id];

    // If this is the first request for this client, window_start will be
    // default-initialized (time_point{}). Treat that as "no window yet".
    if (entry.count == 0) {
        entry.window_start = now;
        entry.count = 1;
        return true;
    }

    const auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - entry.window_start);

    if (elapsed >= window_duration_) {
        // Start a new window.
        entry.window_start = now;
        entry.count = 1;
        return true;
    }

    if (entry.count < max_requests_) {
        ++entry.count;
        return true;
    }

    // Exceeded the limit within the current window.
    return false;
}

