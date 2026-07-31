#include "handler.h"

#include "../utils/utils.h"
#include "../../third_party/json.hpp"

namespace booking {

namespace {

using json = nlohmann::json;

struct SeatInfo {
    std::string seat_id;
    std::string user_id;
    bool booked;
};

void to_json(json& j, const SeatInfo& s) {
    j = json{
        {"seat_id", s.seat_id},
        {"user_id", s.user_id},
        {"booked", s.booked},
    };
}

}  // namespace

void Handler::ListSeats(const httplib::Request& req, httplib::Response& res) {
    std::string movie_id = req.matches[1];

    std::vector<Booking> bookings = svc_->ListBooking(movie_id);

    std::vector<SeatInfo> seats;
    seats.reserve(bookings.size());
    for (const auto& b : bookings) {
        seats.push_back(SeatInfo{b.seat_id, b.user_id, true});
    }

    utils::WriteJSON(res, 200, json(seats));
}

}  // namespace booking