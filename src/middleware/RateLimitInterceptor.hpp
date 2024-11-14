// RateLimitInterceptor.hpp
#ifndef RATE_LIMIT_INTERCEPTOR_HPP
#define RATE_LIMIT_INTERCEPTOR_HPP

#include "RateLimiter.hpp"
#include "oatpp/web/server/interceptor/RequestInterceptor.hpp"
#include "oatpp/web/protocol/http/outgoing/Response.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include <memory>
#include <string>

class RateLimitInterceptor : public oatpp::web::server::interceptor::RequestInterceptor {
public:
    RateLimitInterceptor(std::shared_ptr<RateLimiter> rateLimiter)
        : m_rateLimiter(rateLimiter) {}

    std::shared_ptr<const oatpp::web::protocol::http::outgoing::Response>
    intercept(const std::shared_ptr<oatpp::web::protocol::http::incoming::Request>& request) override {

        // Identify the client (e.g., by IP address)
        auto clientIp = request->getConnection()->getRemoteAddress()->toString();

        int remaining = 0;
        int resetSeconds = 0;
        bool allowed = m_rateLimiter->allowRequest(clientIp, remaining, resetSeconds);

        if (!allowed) {
            // Return 429 Too Many Requests
            auto response = oatpp::web::protocol::http::outgoing::Response::createShared();
            response->setStatusCode(oatpp::web::protocol::http::Status::CODE_429);
            response->putHeader("Content-Type", "text/plain");
            response->putHeader("Retry-After", std::to_string(resetSeconds));
            response->putHeader("X-RateLimit-Limit", std::to_string(m_rateLimiter->getMaxRequests()));
            response->putHeader("X-RateLimit-Remaining", std::to_string(remaining));
            response->putHeader("X-RateLimit-Reset", std::to_string(resetSeconds));
            response->writeSimpleBody("Too Many Requests");
            return response;
        }

        // Optionally, set rate limit headers for allowed requests
        auto response = request->getResponse();
        response->putHeader("X-RateLimit-Limit", std::to_string(m_rateLimiter->getMaxRequests()));
        response->putHeader("X-RateLimit-Remaining", std::to_string(remaining));
        response->putHeader("X-RateLimit-Reset", std::to_string(resetSeconds));

        return nullptr; // Continue processing the request
    }

private:
    std::shared_ptr<RateLimiter> m_rateLimiter;
};

#endif // RATE_LIMIT_INTERCEPTOR_HPP