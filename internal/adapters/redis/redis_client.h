#pragma once

#include <hiredis/hiredis.h>

#include <memory>
#include <string>

namespace redis {

// Thin RAII wrapper around a hiredis connection.
class Client {
public:
    static std::unique_ptr<Client> NewClient(const std::string& addr);

    ~Client();

    redisContext* raw() const { return ctx_; }

private:
    explicit Client(redisContext* ctx) : ctx_(ctx) {}

    redisContext* ctx_;
};

}  // namespace redis