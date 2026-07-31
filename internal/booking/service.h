#pragma once

#include "domain.h"

namespace booking {

class Service {
public:
    explicit Service(BookingStore* store) : store_(store) {}

    Booking Book(const Booking& b) { return store_->Book(b); }

    std::vector<Booking> ListBooking(const std::string& movie_id) {
        return store_->ListBooking(movie_id);
    }

    Booking ConfirmSeat(const std::string& session_id, const std::string& user_id) {
        return store_->Confirm(session_id, user_id);
    }

    void ReleaseSeat(const std::string& session_id, const std::string& user_id) {
        store_->Release(session_id, user_id);
    }

private:
    BookingStore* store_;
};

}  // namespace booking