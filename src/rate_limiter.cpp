#include <iostream>
#include <chrono>

#include "rate_limiter.h"

namespace {
constexpr const char* kFixedWindowAlgorithmName = "FixedWindow";
constexpr const char* kSlidingWindowAlgorithmName = "SlidingWindow";

inline std::int64_t currentTimestampSeconds() {
    const auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(
               now.time_since_epoch())
        .count();
}

inline void logRequest(const std::string& client_id,
                       const char* algorithm_name,
                       bool allowed) {
    const auto ts = currentTimestampSeconds();
    std::cout << "[" << ts << "] client_id=" << client_id
              << " algorithm=" << algorithm_name
              << " result=" << (allowed ? "ALLOWED" : "RATE_LIMITED")
              << '\n';
}
}  // namespace

RateLimiter::RateLimiter(std::size_t max_requests,
                         std::chrono::milliseconds window_duration)
    : max_requests_(max_requests),
      window_duration_(window_duration) {}

bool RateLimiter::allowRequest(const std::string& client_id) {
    const auto now = Clock::now();

    std::lock_guard<std::mutex> lock(mutex_);

    auto& entry = clients_[client_id];
    bool allowed = false;

    // If this is the first request for this client, window_start will be
    // default-initialized (time_point{}). Treat that as "no window yet".
    if (entry.count == 0) {
        entry.window_start = now;
        entry.count = 1;
        allowed = true;
    } else {
        const auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - entry.window_start);

        if (elapsed >= window_duration_) {
            // Start a new window.
            entry.window_start = now;
            entry.count = 1;
            allowed = true;
        } else if (entry.count < max_requests_) {
            ++entry.count;
            allowed = true;
        } else {
            // Exceeded the limit within the current window.
            allowed = false;
        }
    }

    logRequest(client_id, kFixedWindowAlgorithmName, allowed);
    return allowed;
}

bool RateLimiter::allowRequestSliding(const std::string& client_id) {
    const auto now = Clock::now();

    std::lock_guard<std::mutex> lock(mutex_);

    auto& log = sliding_logs_[client_id];

    // Remove timestamps that are outside the current sliding window.
    while (!log.empty()) {
        const auto oldest = log.front();
        const auto age =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - oldest);
        if (age >= window_duration_) {
            log.pop_front();
        } else {
            break;
        }
    }

    bool allowed = false;
    if (log.size() < max_requests_) {
        log.push_back(now);
        allowed = true;
    } else {
        allowed = false;
    }

    logRequest(client_id, kSlidingWindowAlgorithmName, allowed);
    return allowed;
}

