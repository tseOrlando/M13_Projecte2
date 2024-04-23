//
// Created by sel on 15/04/2024.
//

#include "member_t.h"

#ifndef HARD_MOTION_EVENT_H
#define HARD_MOTION_EVENT_H

class event_t
{
public:
    event_t(const std::string &name, const std::string &info, const std::vector<member_t> &members): name(name), info(info), members(members) {}
    virtual ~event_t() {}

    const std::string &get_name() const              { return event_t::name; }
    const std::string &get_info() const              { return event_t::info; }
    const std::vector<member_t> &get_members() const { return event_t::members; }

private:
    std::string name;
    std::string info;

private:
    std::vector<member_t> members;
};

#endif //HARD_MOTION_EVENT_H