## SeatLock
A cinema booking system written in C++ that lets multiple users book tickets concurrently without double-booking a seat.

## Demo
<img width="3018" height="1532" alt="Adobe Express - Screen Recording 2026-07-15 at 9 17 53 AM" src="https://github.com/user-attachments/assets/cb210deb-dd3d-43c1-86c8-1e86709dfacd" />

## Problem Statement
How do we build a fast booking system that prevents double-bookings?

**Strategy 1: Synchronous Approach**
- Users buy tickets in first-come-first-served order at a register.
- This prevents double-booking but is slow, since every purchase blocks the next.

**Strategy 2: Optimistic Concurrency**
- Sell seats online with no lock while a purchase is in progress.
- Although this is faster, two users can end up racing for the same seat — one of them enters their card details only to find the seat was taken.

**Strategy 3: Pessimistic Locking**
- Sell seats online, but when a user starts checkout they're given a short-lived lock on the seat.
- This is the best of both worlds: it's fast, and no one wastes time entering payment details for a seat someone else just took.

SeatLock implements Strategy 3, using Redis as the lock store.

## Getting Started
**Requirements:** a C++17 compiler (g++/clang++), Make, Docker, libhiredis

```bash
# install the hiredis client library
sudo apt install libhiredis-dev   # or: brew install hiredis

# start Redis (+ Redis Commander UI on :8081)
docker compose up -d

# build and run the server
make run
```

The app is served at [http://localhost:8080](http://localhost:8080).

## Testing
Tests hit Redis directly, so make sure it's running first:
```bash
docker compose up -d
make test
```
This runs the concurrent booking test, which fires 100,000 concurrent booking requests at the same seat and asserts exactly one succeeds — the core guarantee the locking strategy is built to provide.
