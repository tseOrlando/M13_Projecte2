//
// Created by sel on 08/05/2024.
//

#include "api_rest_fetch.h"

size_t api_rest_fetch::write_callback(void *contents, size_t size, size_t nmemb, std::string *response) noexcept
{
    size_t total_size = size * nmemb;
    response->append((char*)contents, total_size);
    return total_size;
}

std::string api_rest_fetch::generate_request(const std::string &endpoint, const std::string &request_type, const std::string& post_data) noexcept
{
    CURL *curl = curl_easy_init();
    CURLcode res;
    std::string response_buffer = "";

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, std::string(base_url + endpoint).c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request_type.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_buffer);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        if (request_type == "POST")
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            __android_log_print(ANDROID_LOG_INFO, "curl ", "%s", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }

    return response_buffer;
}

std::string  api_rest_fetch::_get(const std::string& endpoint) noexcept { return generate_request(endpoint, "GET"); }

std::string  api_rest_fetch::_post(const std::string& endpoint, const std::string& data) noexcept { return generate_request(endpoint, "POST", data); }

std::string  api_rest_fetch::_delete(const std::string& endpoint) noexcept { return generate_request(endpoint, "DELETE"); }

std::string api_rest_fetch::_put(const std::string &endpoint) noexcept { return generate_request(endpoint, "PUT"); }

std::string api_rest_fetch::get_latest_id(const std::string &from) noexcept { return _get("/" + from + "/latest"); }

std::vector<event_t> api_rest_fetch::get_events() noexcept
{
    std::vector<event_t> events_requested;
    json data_requested = json::parse(_get("/events"));

    for (const auto& data : data_requested)
        events_requested.emplace_back(event_t(data));

    return events_requested;
}

event_t api_rest_fetch::get_event(const std::string &id) noexcept { return event_t(json::parse(_get("/event/" + id))); }

bool api_rest_fetch::post_event(event_t event_to_post) noexcept
{
    int events_latest_id = std::atoi(get_latest_id("events").c_str());

    json j;

    j["_id"]     = std::to_string(events_latest_id + 1);
    j["title"]   = event_to_post.get_title();
    j["info"]    = event_to_post.get_info();
    j["members"] = json::array();

    return _post("/event", j.dump()).find("true") != std::string::npos;
}

std::string api_rest_fetch::delete_event(const std::string &id) noexcept { return _delete("/event/" + id); }

std::vector<member_t> api_rest_fetch::get_members_from_event(const std::string event_id) noexcept
{
    std::vector<member_t> members_requested;

    json data_requested = json::parse(_get("/event_members/" + event_id));

    if (data_requested.contains("members"))
        for (const auto& member_id : data_requested["members"])
            members_requested.push_back(get_member(member_id));

    return members_requested;
}

member_t api_rest_fetch::get_member(const std::string &id) noexcept { return member_t(json::parse(_get("/member/" + id))); }

std::vector<member_t> api_rest_fetch::get_members() noexcept
{
    std::vector<member_t> events_requested;
    json data_requested = json::parse(_get("/members"));

    for (const auto& data : data_requested)
        events_requested.emplace_back(member_t(data));

    return events_requested;
}

/*
 * post_member and _event could be optimized but i'm lazy to do a template and just parameter a json object
 * if it were a real app for me i would..
 */
bool api_rest_fetch::post_member(member_t member_to_post) noexcept
{
    int members_latest_id = std::atoi(get_latest_id("members").c_str());

    json j;

    j["_id"]         = std::to_string(members_latest_id + 1);
    j["name"]        = member_to_post.get_name();
    j["number"]      = member_to_post.get_number();
    j["email"]       = member_to_post.get_e_mail();
    j["dance_type"]  = member_to_post.get_dance_type();
    j["password"]    = member_to_post.get_password();
    j["events"]      = json::array();

    return _post("/member", j.dump()).find("true") != std::string::npos;
}

std::string api_rest_fetch::delete_member(const std::string &id) noexcept { return _delete("/event/" + id); }

std::pair<bool, member_t>  api_rest_fetch::get_member_by_name(const std::string &name) noexcept
{
    json requested_member_raw = json::parse(_get("/member_name/" + name));

    if (not requested_member_raw.contains("detail") and not requested_member_raw.contains("error"))
        return std::make_pair(true, member_t(requested_member_raw));

    return std::make_pair(false, member_t("error", "error", "error", "error", "error"));
}

std::vector<event_t> api_rest_fetch::get_events_from_member(const std::string member_id) noexcept
{
    std::vector<event_t> events_requested;

    json data_requested = json::parse(_get("/member_events/" + member_id));

    if (data_requested.contains("events"))
        for (const auto& event_id : data_requested["events"])
            events_requested.push_back(get_event(member_id));

    return events_requested;
}

bool api_rest_fetch::join_member_to_event(const std::string member_id, const std::string event_id) noexcept
{
    json result = json::parse(_put("/member/" + member_id + "/join/" + event_id));

    if (result.contains("true"))
        if (result["true"] == "true")
            return true;

    return false;
}
