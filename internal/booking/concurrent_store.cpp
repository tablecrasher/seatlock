#include "concurrent_store.h"

#include <mutex>

namespace booking {

void ConcurrentStore::Book(const Booking& b) {
    std::unique_lock lock(mu_);

    if (bookings_.count(b.seat_id)) {
        throw SeatAlreadyBookedError();
    }
    bookings_[b.seat_id] = b;
}

std::vector<Booking> ConcurrentStore::ListBooking(const std::string& movie_id) {
    std::shared_lock lock(mu_);

    std::vector<Booking> result;
    for (const auto& [seat_id, b] : bookings_) {
        if (b.movie_id == movie_id) {
            result.push_back(b);
        }
    }
    return result;
}

}  // namespace booking