#pragma once

#include <random>
#include <sstream>
#include <string>

namespace booking {

// Generates a random RFC-4122 v4 UUID string, e.g. "3fa85f64-5717-4562-b3fc-2c963f66afa6".
inline std::string NewUUID() {
    static thread_local std::mt19937_64 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, 15);
    static const char* hex = "0123456789abcdef";

    std::string s(36, '-');
    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) continue;
        s[i] = hex[dist(rng)];
    }
    s[14] = '4';  // version
    s[19] = hex[8 + (dist(rng) % 4)];  // variant

    return s;
}

}  // namespace booking