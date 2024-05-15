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

    member_t(const std::string &id, const std::string &name, const std::string& password, const std::string &number, const std::string& dance_type) : id(id), name(name), password(password), number(number), dance_type(dance_type) {}
    member_t(json j)
    {
        id = j["_id"].get<std::string>();
        name = j["name"].get<std::string>();
        //password = j["password"].get<std::string>(); for security reasons we won't get it
        number = j["number"].get<std::string>();
        e_mail = j["email"].get<std::string>();
        dance_type = j["dance_type"].get<std::string>();
    }

    member_t() {}
    virtual ~member_t() {}

    const std::string &get_name() const        { return name; }

    const std::string &get_number() const      { return number; }
    const std::string &get_e_mail() const      { return e_mail; }

    const std::string &get_dance_type()        { return dance_type; }

    //not safe but idc xd
    dance_type_e dance_type_native(const std::string& dance_type)
    {
        if (dance_type.find("jumpstyle") != std::string::npos) return dance_type_e::jumpstyle;
        else if (dance_type.find("hakken_gabber") != std::string::npos) return dance_type_e::hakken_gabber;
        else return dance_type_e::shuffle;
    }

    //altrough if this were a real app, i would take measurements, for security purposes i wouldn't even
    //store the users password in the instance, it would be encrypted by a basic xor or more advanced techniques
    bool save()
    {
        int latest_id = std::atoi(api_rest_fetch::get_latest_id("members").c_str());

        json j;
        j["_id"] = std::to_string(latest_id + 1);
        j["name"] = name;
        j["password"] = password;
        j["number"] = number;
        j["email"] = e_mail;
        j["dance_type"] = dance_type;

        std::string inserted_id = api_rest_fetch::_post(api_rest_fetch::base_url + "/member", to_string(j));

        return inserted_id != std::to_string(latest_id); //if added return true, else false
    }

private:
    std::string id;
    std::string name;
    std::string password;
    std::string number;
    std::string e_mail;
    std::string dance_type;
};

#endif //HARD_MOTION_EVENT_H