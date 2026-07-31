#pragma once

#include "../../third_party/httplib.h"
#include "service.h"

namespace booking {

class Handler {
public:
    explicit Handler(Service* svc) : svc_(svc) {}

    void ListSeats(const httplib::Request& req, httplib::Response& res);
    void HoldSeat(const httplib::Request& req, httplib::Response& res);
    void ConfirmSession(const httplib::Request& req, httplib::Response& res);
    void ReleaseSession(const httplib::Request& req, httplib::Response& res);

private:
    Service* svc_;
};

}  // namespace booking