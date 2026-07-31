#pragma once

#include <chrono>
#include <mutex>
#include <utility>

#include "adapters/redis/redis_client.h"
#include "domain.h"

namespace booking {

constexpr std::chrono::minutes kDefaultHoldTTL{2};

// RedisStore implements session-based seat booking backed by Redis.
//
// Key design:
//
//   seat:{movieID}:{seatID}   -> session JSON (TTL = held, no TTL = confirmed)
//   session:{sessionID}       -> seat key     (reverse lookup)
class RedisStore : public BookingStore {
public:
    explicit RedisStore(redis::Client* rdb) : rdb_(rdb) {}

    Booking Book(const Booking& b) override;
    std::vector<Booking> ListBooking(const std::string& movie_id) override;

    // Confirm converts a held session into a permanent booking.
    // Removes the TTL (PERSIST) so the key never expires.
    Booking Confirm(const std::string& session_id, const std::string& user_id) override;
    void Release(const std::string& session_id, const std::string& user_id) override;

private:
    Booking hold(Booking b);
    // getSession resolves a session id to its booking and underlying seat key.
    std::pair<Booking, std::string> getSession(const std::string& session_id,
                                                const std::string& user_id);

    redis::Client* rdb_;
    std::mutex conn_mu_;  // hiredis connections aren't safe to share across threads
};

}  // namespace booking