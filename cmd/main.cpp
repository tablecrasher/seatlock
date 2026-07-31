#include <iostream>

#include "../internal/adapters/redis/redis_client.h"
#include "../internal/booking/handler.h"
#include "../internal/booking/redis_store.h"
#include "../internal/booking/service.h"
#include "../internal/utils/utils.h"
#include "../third_party/httplib.h"
#include "../third_party/json.hpp"

using json = nlohmann::json;

struct MovieResponse {
    std::string id;
    std::string title;
    int rows;
    int seats_per_row;
};

void to_json(json& j, const MovieResponse& m) {
    j = json{
        {"id", m.id},
        {"title", m.title},
        {"rows", m.rows},
        {"seats_per_row", m.seats_per_row},
    };
}

static const std::vector<MovieResponse> movies = {
    {"obsession", "Obsession", 5, 8},
    {"jurassicworld", "Jurassic World", 4, 6},
};

void listMovies(const httplib::Request&, httplib::Response& res) {
    utils::WriteJSON(res, 200, json(movies));
}

int main() {
    httplib::Server svr;

    svr.Get("/movies", listMovies);

    svr.set_mount_point("/", "./static");

    auto rdb = redis::Client::NewClient("localhost:6379");
    booking::RedisStore store(rdb.get());
    booking::Service svc(&store);

    booking::Handler bookingHandler(&svc);

    svr.Get(R"(/movies/([^/]+)/seats)",
            [&](const httplib::Request& req, httplib::Response& res) {
                bookingHandler.ListSeats(req, res);
            });

    svr.Post(R"(/movies/([^/]+)/seats/([^/]+)/hold)",
             [&](const httplib::Request& req, httplib::Response& res) {
                 bookingHandler.HoldSeat(req, res);
             });

    svr.Put(R"(/sessions/([^/]+)/confirm)",
            [&](const httplib::Request& req, httplib::Response& res) {
                bookingHandler.ConfirmSession(req, res);
            });

    svr.Delete(R"(/sessions/([^/]+))",
               [&](const httplib::Request& req, httplib::Response& res) {
                   bookingHandler.ReleaseSession(req, res);
               });

    std::cout << "listening on :8080" << std::endl;
    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "failed to start server" << std::endl;
        return 1;
    }
}