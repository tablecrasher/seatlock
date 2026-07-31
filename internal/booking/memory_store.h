#pragma once

#include <unordered_map>

#include "domain.h"

namespace booking {

class MemoryStore : public BookingStore {
public:
    MemoryStore() = default;

    Booking Book(const Booking& b) override;
    std::vector<Booking> ListBooking(const std::string& movie_id) override;

private:
    std::unordered_map<std::string, Booking> bookings_;  // "A2" -> booking
};

}  // namespace booking