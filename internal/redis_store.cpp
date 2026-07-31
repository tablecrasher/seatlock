#include "redis_store.h"

#include <hiredis/hiredis.h>

#include <iostream>

#include "../third_party/json.hpp"
#include "uuid.h"

namespace booking {

namespace {

using json = nlohmann::json;

std::string toJSON(const Booking& b) {
    json j;
    j["id"] = b.id;
    j["movie_id"] = b.movie_id;
    j["seat_id"] = b.seat_id;
    j["user_id"] = b.user_id;
    j["status"] = b.status;
    return j.dump();
}

}  // namespace

void RedisStore::Book(const Booking& b) {
    Booking session = hold(b);
    std::cout << "Session booked " << session.id << std::endl;
}

std::vector<Booking> RedisStore::ListBooking(const std::string& /*movie_id*/) {
    return {};
}

Booking RedisStore::hold(Booking b) {
    std::string id = NewUUID();
    auto now = std::chrono::system_clock::now();
    std::string key = "seat:" + b.movie_id + ":" + b.seat_id;

    b.id = id;
    std::string val = toJSON(b);

    long long ttl_seconds =
        std::chrono::duration_cast<std::chrono::seconds>(kDefaultHoldTTL).count();

    std::lock_guard<std::mutex> lock(conn_mu_);

    redisReply* reply = static_cast<redisReply*>(redisCommand(
        rdb_->raw(), "SET %s %s NX EX %lld", key.c_str(), val.c_str(), ttl_seconds));
    bool ok = reply != nullptr && reply->type == REDIS_REPLY_STATUS;
    if (reply) freeReplyObject(reply);

    if (!ok) {
        throw SeatAlreadyBookedError();
    }

    std::string sk = "session:" + id;
    redisReply* setSession = static_cast<redisReply*>(
        redisCommand(rdb_->raw(), "SET %s %s EX %lld", sk.c_str(), key.c_str(), ttl_seconds));
    if (setSession) freeReplyObject(setSession);

    Booking result;
    result.id = id;
    result.movie_id = b.movie_id;
    result.seat_id = b.seat_id;
    result.user_id = b.user_id;
    result.status = "held";
    result.expires_at = now + kDefaultHoldTTL;
    return result;
}

}  // namespace booking