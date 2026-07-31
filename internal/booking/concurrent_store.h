#pragma once

#include <shared_mutex>
#include <unordered_map>

#include "domain.h"

namespace booking {

class ConcurrentStore : public BookingStore {
public:
    ConcurrentStore() = default;

    Booking Book(const Booking& b) override;
    std::vector<Booking> ListBooking(const std::string& movie_id) override;

private:
    std::shared_mutex mu_;
    std::unordered_map<std::string, Booking> bookings_;
};

}  // namespace booking