#pragma once

#include "domain.h"

namespace booking {

class Service {
public:
    explicit Service(BookingStore* store) : store_(store) {}

    void Book(const Booking& b) { store_->Book(b); }

private:
    BookingStore* store_;
};

}  // namespace booking