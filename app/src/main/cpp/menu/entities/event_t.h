//
// Created by sel on 15/04/2024.
//

#include "member_t.h"

#ifndef HARD_MOTION_EVENT_H
#define HARD_MOTION_EVENT_H

class event_t
{
public:
    event_t(const std::string &id, const std::string &title, const std::string &info, const std::vector<std::string> &members = {}): id(id), title(title), info(info), members(members) {}
    event_t(json j)
    {
        id = j["_id"].get<std::string>();
        title = j["title"].get<std::string>();
        info = j["info"].get<std::string>();

        for (const auto& member : j["members"])
            members.push_back(member.get<std::string>());
    }

    event_t() {}
    virtual ~event_t() {}

    const std::string &get_id()   const              { return event_t::id; }
    const std::string &get_title() const              { return event_t::title; }
    const std::string &get_info() const              { return event_t::info; }
    const std::vector<std::string> &get_members() const { return event_t::members; }

    bool save()
    {
        int latest_id = std::atoi(api_rest_fetch::get_latest_id("events").c_str());

        json j;
        j["_id"] = std::to_string(latest_id + 1);
        j["title"] = title;
        j["info"] = info;
        j["members"] = members;

        std::string inserted_id = api_rest_fetch::_post(api_rest_fetch::base_url + "/event", to_string(j));

        return inserted_id != std::to_string(latest_id); //if added return true, else false
    }

private:
    std::string id;
    std::string title;
    std::string info;
    std::vector<std::string> members;
};

#endif //HARD_MOTION_EVENT_H