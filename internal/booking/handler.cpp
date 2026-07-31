#include "handler.h"

#include <ctime>
#include <iostream>

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

// formatRFC3339 renders a time_point the way Go's time.RFC3339 layout does,
// e.g. "2026-07-29T18:30:00Z".
std::string formatRFC3339(std::chrono::system_clock::time_point tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
    gmtime_r(&t, &tm);

    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm);
    return buf;
}

}  // namespace

void Handler::HoldSeat(const httplib::Request& req, httplib::Response& res) {
    std::string movie_id = req.matches[1];
    std::string seat_id = req.matches[2];

    json body;
    try {
        body = json::parse(req.body);
    } catch (const json::parse_error& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    Booking data;
    data.user_id = body.value("user_id", "");
    data.seat_id = seat_id;
    data.movie_id = movie_id;

    Booking session;
    try {
        session = svc_->Book(data);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }

    json holdResponse = {
        {"session_id", session.id},
        {"movieID", session.movie_id},
        {"seat_id", seat_id},
        {"expires_at", formatRFC3339(session.expires_at)},
    };

    utils::WriteJSON(res, 201, holdResponse);
}

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