#include "redis_client.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

namespace redis {

std::unique_ptr<Client> Client::NewClient(const std::string& addr) {
    auto sep = addr.find(':');
    std::string host = addr.substr(0, sep);
    int port = std::stoi(addr.substr(sep + 1));

    redisContext* ctx = redisConnect(host.c_str(), port);
    if (ctx == nullptr || ctx->err) {
        if (ctx) {
            std::cerr << "redis ping " << ctx->errstr << std::endl;
        } else {
            std::cerr << "redis ping: can't allocate redis context" << std::endl;
        }
        std::exit(1);
    }

    redisReply* reply = static_cast<redisReply*>(redisCommand(ctx, "PING"));
    if (reply == nullptr) {
        std::cerr << "redis ping: no reply" << std::endl;
        std::exit(1);
    }
    freeReplyObject(reply);

    std::cout << "connected to redis at " << addr << std::endl;

    return std::unique_ptr<Client>(new Client(ctx));
}

Client::~Client() {
    if (ctx_) redisFree(ctx_);
}

}  // namespace redis