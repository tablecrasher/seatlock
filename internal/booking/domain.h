#pragma once

#include <chrono>
#include <stdexcept>
#include <string>
#include <vector>

namespace booking {

// Thrown when a session id doesn't resolve to a live hold.
class SessionNotFoundError : public std::runtime_error {
public:
    SessionNotFoundError() : std::runtime_error("session not found") {}
};

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
    std::chrono::system_clock::time_point expires_at;
};

class BookingStore {
public:
    virtual ~BookingStore() = default;

    virtual Booking Book(const Booking& b) = 0;
    virtual std::vector<Booking> ListBooking(const std::string& movie_id) = 0;

    virtual Booking Confirm(const std::string& session_id, const std::string& user_id) = 0;
    virtual void Release(const std::string& session_id, const std::string& user_id) = 0;
};

}  // namespace booking