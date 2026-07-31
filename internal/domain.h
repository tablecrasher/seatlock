#pragma once

#include <stdexcept>
#include <string>
#include <vector>

namespace booking {

// Thrown when a seat is already taken.
class SeatAlreadyBookedError : public std::runtime_error {
public:
    SeatAlreadyBookedError() : std::runtime_error("seat is already taken") {}
};

// Booking represents a confirmed seat reservation.
struct Booking {
    std::string id;
    std::string movie_id;
    std::string seat_id;
    std::string user_id;
    std::string status;
};

class BookingStore {
public:
    virtual ~BookingStore() = default;

    virtual void Book(const Booking& b) = 0;
    virtual std::vector<Booking> ListBooking(const std::string& movie_id) = 0;
};

}  // namespace booking