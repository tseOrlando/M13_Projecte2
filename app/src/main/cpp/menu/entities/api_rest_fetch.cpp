//
// Created by sel on 08/05/2024.
//

#include "main/cpp/menu/headers/menu.h"

#include "api_rest_fetch.h"

size_t api_rest_fetch::write_callback(void *contents, size_t size, size_t nmemb, std::string *response) noexcept
{
    size_t total_size = size * nmemb;
    response->append((char*)contents, total_size);
    return total_size;
}

std::string api_rest_fetch::generate_request(const std::string &url, const std::string &request_type, const std::string& post_data) noexcept
{


    return "";
}

std::string api_rest_fetch::_get(const std::string& url) noexcept { return generate_request(url, "GET"); }

std::string api_rest_fetch::_post(const std::string& url, const std::string& data) noexcept { return generate_request(url, "POST", data); }

std::string api_rest_fetch::_delete(const std::string& url) noexcept { return generate_request(url, "DELETE"); }

std::string api_rest_fetch::get_latest_id(const std::string &from) noexcept { return _get(base_url + from + "/latest"); }

std::string api_rest_fetch::get_events() noexcept { return _get(base_url + "/events"); }

std::string api_rest_fetch::get_members() noexcept { return _get(base_url + "/members"); }

std::string api_rest_fetch::delete_event(const std::string &id) noexcept { return _delete(base_url + "/event/" + id); }

std::string api_rest_fetch::delete_member(const std::string &id) noexcept { return _delete(base_url + "/event/" + id); }

std::string api_rest_fetch::get_member_password(const std::string &name) noexcept { return _get(base_url + "member_password/" + name); }
