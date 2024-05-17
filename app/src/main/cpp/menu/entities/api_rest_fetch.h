//
// Created by sel on 07/05/2024.
//

#ifndef HARD_MOTION_API_REST_FETCH_H
#define HARD_MOTION_API_REST_FETCH_H

#include <curl/include/curl/curl.h>

namespace api_rest_fetch
{
    inline std::string base_url = "http://192.168.0.27:8000";

    size_t write_callback(void *contents, size_t size, size_t nmemb, std::string *response) noexcept;

    std::string generate_request(const std::string& url, const std::string& request_type, const std::string& post_data = "") noexcept;

    std::string _get(const std::string& url) noexcept;
    std::string _post(const std::string& url, const std::string& data) noexcept;
    std::string _delete(const std::string& url) noexcept;

    std::string get_latest_id(const std::string &from) noexcept;
    std::string get_events() noexcept;
    std::string get_members() noexcept;

    std::string delete_event(const std::string& id) noexcept;
    std::string delete_member(const std::string& id) noexcept;

    std::string get_member_password(const std::string& name) noexcept;
}

#endif //HARD_MOTION_API_REST_FETCH_H
