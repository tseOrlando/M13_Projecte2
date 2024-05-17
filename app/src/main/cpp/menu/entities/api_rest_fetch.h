//
// Created by sel on 07/05/2024.
//

#ifndef HARD_MOTION_API_REST_FETCH_H
#define HARD_MOTION_API_REST_FETCH_H

#include "../../global_includes/ginc.h"

#include <curl/curl.h>

using json = nlohmann::ordered_json;

class member_t
{
public:
    enum class dance_type_e : int              { jumpstyle = 0, hakken_gabber = 1, shuffle = 2 };

    member_t(const std::string &name, const std::string& password, const std::string &number, const std::string &e_mail, const std::string& dance_type) : name(name), password(password), number(number), e_mail(e_mail), dance_type(dance_type) {}
    member_t(json j)
    {
        id = j["_id"].get<std::string>();
        name = j["name"].get<std::string>();
        password = j["password"].get<std::string>();
        number = j["number"].get<std::string>();
        e_mail = j["email"].get<std::string>();
        dance_type = j["dance_type"].get<std::string>();
    }

    member_t() {}
    virtual ~member_t() {}

    const std::string &get_id() const        { return id; }
    const std::string &get_name() const        { return name; }
    const std::string &get_password() const      { return password; }

    const std::string &get_number() const      { return number; }
    const std::string &get_e_mail() const      { return e_mail; }

    const std::string &get_dance_type()        { return dance_type; }

    //in case we need it, not safe but idc xd
    dance_type_e dance_type_native(const std::string& dance_type_)
    {
        if (dance_type_.find("jumpstyle") != std::string::npos) return dance_type_e::jumpstyle;
        else if (dance_type_.find("hakken_gabber") != std::string::npos) return dance_type_e::hakken_gabber;
        else return dance_type_e::shuffle;
    }

private:
    std::string id;
    std::string name;
    std::string password;
    std::string number;
    std::string e_mail;
    std::string dance_type;
};

class event_t
{
public:
    event_t(const std::string &title, const std::string &info, const std::vector<std::string> &members = {}): id(id), title(title), info(info), members(members) {}
    event_t(json j)
    {
        id    = j["_id"].get<std::string>();
        title = j["title"].get<std::string>();
        info  = j["info"].get<std::string>();

        for (const auto& member : j["members"])
            members.push_back(member.get<std::string>());
    }

    event_t() {}
    virtual ~event_t() {}

    const std::string &get_id()   const              { return event_t::id; }
    const std::string &get_title() const              { return event_t::title; }
    const std::string &get_info() const              { return event_t::info; }
    const std::vector<std::string> &get_members() const { return event_t::members; }

private:
    std::string id;
    std::string title;
    std::string info;
    std::vector<std::string> members;
};

namespace api_rest_fetch
{
    inline std::string base_url = "http://192.168.0.26:8000";

    size_t write_callback(void *contents, size_t size, size_t nmemb, std::string *response) noexcept;

    std::string  generate_request(const std::string& endpoint, const std::string& request_type, const std::string& post_data = "") noexcept;

    std::string  _get(const std::string& endpoint) noexcept;
    std::string  _post(const std::string& endpoint, const std::string& data) noexcept;
    std::string  _delete(const std::string& endpoint) noexcept;
    std::string  _put(const std::string& endpoint) noexcept;

    std::string get_latest_id(const std::string &from) noexcept;

    std::vector<event_t> get_events() noexcept;
    event_t get_event(const std::string& id) noexcept;
    bool post_event(event_t event_to_post) noexcept;
    std::string delete_event(const std::string& id) noexcept;
    std::vector<member_t> get_members_from_event(const std::string event_id) noexcept;

    member_t get_member(const std::string& id) noexcept;
    std::vector<member_t> get_members() noexcept;
    bool post_member(member_t member_to_post) noexcept;
    std::string delete_member(const std::string& id) noexcept;
    std::pair<bool, member_t> get_member_by_name(const std::string& name) noexcept;
    std::vector<event_t> get_events_from_member(const std::string member_id) noexcept;

    bool join_member_to_event(const std::string member_id, const std::string event_id) noexcept;
}

#endif //HARD_MOTION_API_REST_FETCH_H
