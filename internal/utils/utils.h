#pragma once

#include "../../third_party/httplib.h"
#include "../../third_party/json.hpp"

namespace utils {

inline void WriteJSON(httplib::Response& res, int status, const nlohmann::json& v) {
    res.status = status;
    res.set_content(v.dump(), "application/json");
}

}  // namespace utils