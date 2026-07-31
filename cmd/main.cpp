#include <iostream>

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

void WriteJSON(httplib::Response& res, int status, const json& v) {
    res.status = status;
    res.set_content(v.dump(), "application/json");
}

void listMovies(const httplib::Request&, httplib::Response& res) {
    WriteJSON(res, 200, json(movies));
}

int main() {
    httplib::Server svr;

    svr.Get("/movies", listMovies);
    
    svr.set_mount_point("/", "./static");

    std::cout << "listening on :8080" << std::endl;
    if (!svr.listen("0.0.0.0", 8080)) {
        std::cerr << "failed to start server" << std::endl;
        return 1;
    }
}