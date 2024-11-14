// RateLimiter.hpp
#ifndef RATE_LIMITER_HPP
#define RATE_LIMITER_HPP

#include <unordered_map>
#include <mutex>
#include <chrono>
#include <string>

class RateLimiter {
public:
    RateLimiter(int maxRequests, int windowSeconds)
        : m_maxRequests(maxRequests), m_windowSeconds(windowSeconds) {}

    bool allowRequest(const std::string& clientId, int& remaining, int& resetSeconds) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto now = std::chrono::steady_clock::now();

        auto& clientData = m_clients[clientId];
        if (std::chrono::duration_cast<std::chrono::seconds>(now - clientData.windowStart).count() > m_windowSeconds) {
            clientData.windowStart = now;
            clientData.requestCount = 1;
            remaining = m_maxRequests - 1;
            resetSeconds = m_windowSeconds;
            return true;
        } else {
            if (clientData.requestCount < m_maxRequests) {
                clientData.requestCount++;
                remaining = m_maxRequests - clientData.requestCount;
                resetSeconds = m_windowSeconds - static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - clientData.windowStart).count());
                return true;
            } else {
                remaining = 0;
                resetSeconds = m_windowSeconds - static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(now - clientData.windowStart).count());
                return false;
            }
        }
    }

    int getMaxRequests() const {
        return m_maxRequests;
    }

private:
    struct ClientData {
        std::chrono::steady_clock::time_point windowStart = std::chrono::steady_clock::now();
        int requestCount = 0;
    };

    int m_maxRequests;
    int m_windowSeconds;
    std::unordered_map<std::string, ClientData> m_clients;
    std::mutex m_mutex;
};

#endif // RATE_LIMITER_HPP