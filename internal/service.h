#pragma once

#include "domain.h"
#include "redis_store.h"

namespace booking {

class Service {
public:
    explicit Service(RedisStore* store) : store_(store) {}

    void Book(const Booking& b) { store_->Book(b); }

private:
    RedisStore* store_;
};

}  // namespace booking