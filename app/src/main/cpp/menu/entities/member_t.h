//
// Created by sel on 15/04/2024.
//

#ifndef HARD_MOTION_MEMBER_H
#define HARD_MOTION_MEMBER_H

class member_t
{
public:
    const std::string &get_id() const          { return id; }
    void set_id(const std::string &id)         { member_t::id = id; }

    const std::string &get_name() const        { return name; }
    void set_name(const std::string &name)     { member_t::name = name; }

    const std::string &get_number() const      { return number; }
    void set_number(const std::string &number) { member_t::number = number; }

private:
    std::string id;
    std::string name;
    std::string number;
};

#endif //HARD_MOTION_EVENT_H