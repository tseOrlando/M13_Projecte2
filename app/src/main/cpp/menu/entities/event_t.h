//
// Created by sel on 15/04/2024.
//

#include "member_t.h"

class event_t
{
public:
    const std::string &get_name() const              { return name; }
    const std::string &get_info() const              { return info; }
    const std::vector<member_t> &get_members() const { return members; }

private:
    std::string name;
    std::string info;
    std::vector<member_t> members;
};