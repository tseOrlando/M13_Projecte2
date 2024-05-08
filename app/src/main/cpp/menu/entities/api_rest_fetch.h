//
// Created by sel on 07/05/2024.
//

#ifndef HARD_MOTION_API_REST_FETCH_H
#define HARD_MOTION_API_REST_FETCH_H

#include "../net/curl-android-jahrome/include/curl/curl.h"

namespace api_rest_fetch
{
    inline CURL * peer = nullptr;
    inline std::string base_url = "127.0.0.1:8000/";

    bool  init()

    std::string _get(const std::string& url) noexcept;
    std::string _post(const std::string& url, const std::string& data) noexcept;
    std::string _delete(const std::string& url) noexcept;

}

#endif //HARD_MOTION_API_REST_FETCH_H
