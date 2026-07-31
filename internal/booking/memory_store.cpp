#include "memory_store.h"

namespace booking {

void MemoryStore::Book(const Booking& b) {
    if (bookings_.count(b.seat_id)) {
        throw SeatAlreadyBookedError();
    }
    bookings_[b.seat_id] = b;
}

std::vector<Booking> MemoryStore::ListBooking(const std::string& movie_id) {
    std::vector<Booking> result;
    for (const auto& [seat_id, b] : bookings_) {
        if (b.movie_id == movie_id) {
            result.push_back(b);
        }
    }
    return result;
}

}  // namespace booking