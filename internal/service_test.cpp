#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../third_party/doctest.h"

#include <atomic>
#include <thread>
#include <vector>

#include "concurrent_store.h"
#include "service.h"

// 100k "users" try to book the same seat at the same time. Work is spread
// across a fixed pool of worker threads (spawning 100k real OS threads isn't
// practical), but every one of the 100k attempts still goes through
// svc.Book() concurrently with the others.
TEST_CASE("concurrent booking: exactly one wins") {
    booking::ConcurrentStore store;
    booking::Service svc(&store);

    const int num_attempts = 100'000;
    std::atomic<int> next{0};
    std::atomic<int64_t> successes{0};
    std::atomic<int64_t> failures{0};

    auto worker = [&] {
        for (;;) {
            int i = next.fetch_add(1);
            if (i >= num_attempts) break;

            try {
                svc.Book(booking::Booking{
                    .movie_id = "screen-1",
                    .seat_id = "A1",
                    .user_id = "user-" + std::to_string(i),
                });
                successes.fetch_add(1);
            } catch (const booking::SeatAlreadyBookedError&) {
                failures.fetch_add(1);
            }
        }
    };

    unsigned n = std::max(2u, std::thread::hardware_concurrency());
    std::vector<std::thread> pool;
    for (unsigned i = 0; i < n; i++) pool.emplace_back(worker);
    for (auto& t : pool) t.join();

    CHECK(successes.load() == 1);
    CHECK(failures.load() == num_attempts - 1);
}