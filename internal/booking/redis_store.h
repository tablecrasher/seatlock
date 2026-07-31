#pragma once

#include <chrono>
#include <mutex>

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

    void Book(const Booking& b) override;
    std::vector<Booking> ListBooking(const std::string& movie_id) override;

private:
    Booking hold(Booking b);

    redis::Client* rdb_;
    std::mutex conn_mu_;  // hiredis connections aren't safe to share across threads
};

}  // namespace booking