//
// Created by sel on 15/04/2024.
//

#ifndef HARD_MOTION_MEMBER_H
#define HARD_MOTION_MEMBER_H

using namespace nlohmann;

class member_t
{
public:
    enum class dance_type_e : int              { jumpstyle = 0, hakken_gabber = 1, shuffle = 2 };

    member_t(const std::string &id, const std::string &name, const std::string &number, dance_type_e dance_type) : id(id), name(name), number(number), dance_type(dance_type) {}
    member_t() {}
    virtual ~member_t() {}

    const std::string &get_name() const        { return name; }

    const std::string &get_number() const      { return number; }
    const std::string &get_e_mail() const      { return e_mail; }

    const dance_type_e get_dance_type()        { return dance_type; }
    std::string dance_type_str()
    {
        switch (dance_type)
        {
            case dance_type_e::jumpstyle:     return "jumpstyle";
            case dance_type_e::hakken_gabber: return "hakken/gabber";
            case dance_type_e::shuffle:       return "shuffle";
        }
    }

    json save()
    {
        int latest_id = std::atoi(api_rest_fetch::_get(api_rest_fetch::base_url + "members/latest").c_str());

        json j;
        j["_id"] = std::to_string(latest_id + 1);
        j["name"] = name;
        j["number"] = number;
        j["e_mail"] = e_mail;
        j["dance_type"] = dance_type_str();

        api_rest_fetch::_post(api_rest_fetch::base_url + "/member", to_string(j));
    }

private:
    std::string id;
    std::string name;
    std::string number;
    std::string e_mail;
    dance_type_e dance_type;
};

#endif //HARD_MOTION_EVENT_H