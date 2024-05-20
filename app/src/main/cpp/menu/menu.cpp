//
// Created by sel on 21/03/2024.
//

#include "../menu/headers/menu.h"
#include "headers/memory_images/hard_motion_logo.h"

/*
 * TODO: Call core::go_back directly and remove retarded core::change_tab flag
 * TODO: Fix the resume of android phone stuff to save state
 */

/*
 * Here the menu is being built with the ImGui API and it's being already called
 * in 'main.cpp -> MainLaunch()', so once you launch Hard Motion you'll see the
 * result of this block of code.
 */
void menu::spawn() noexcept
{
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));

    ImVec2 display_size = ImGui::GetIO().DisplaySize;

    // Align the window to the display size entirely
    ImGui::SetNextWindowPos(ImVec2(-3, -3));
    ImGui::SetNextWindowSize(ImVec2(display_size.x + 5, display_size.y + 5));

    ImGui::Begin("Hard Motion", nullptr, flags);

    core::go_to_tab(core::current_tab);

    /*
     * Ghetto fix, it would be easier to call 'core::go_back' in the JNI function but it crashes
     * so well, this is how I fixed it, I don't know the problem but R.I.P good code
     */
    if (core::change_tab)
    {
        core::go_back();
        core::change_tab = false;
    }

    ImGui::End();

    ImGui::PopStyleVar();
}

/*
 * This function loads the style applied to 'menu::spawn' with the ImGui API, it's
 * already being called in 'main.cpp -> Init(struct android_app* app)'
 */
void menu::load_style() noexcept
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImFontConfig icons_config;
    icons_config.FontDataOwnedByAtlas = false;
    icons_config.MergeMode            = true;
    icons_config.PixelSnapH           = true;
    icons_config.OversampleH          = static_cast<int>(2.5);
    icons_config.OversampleV          = static_cast<int>(2.5);

    ImVec2 display_size = ImGui::GetIO().DisplaySize;

    float scaled_font_size = scales::scale; // base

    scaled_font_size *= (((display_size.x / display_size.y) < 1.5) ? 0.8f : 1.2f);

    static const unsigned short icons_ranges[] = { 0xe005, 0xf8ff, 0 };

    font::head = io.Fonts->AddFontFromMemoryTTF(&man_rope, sizeof man_rope, scaled_font_size + 100.f);
    io.Fonts->AddFontFromMemoryCompressedTTF(icons_data, icons_size, scaled_font_size - 15.f, &icons_config, icons_ranges);

    font::body = io.Fonts->AddFontFromMemoryTTF(&man_rope, sizeof man_rope, scaled_font_size + 20.f);
    io.Fonts->AddFontFromMemoryCompressedTTF(icons_data, icons_size, scaled_font_size - 35.f, &icons_config, icons_ranges);

    font::foot = io.Fonts->AddFontFromMemoryTTF(&man_rope, sizeof man_rope, scaled_font_size - 20.f);
    io.Fonts->AddFontFromMemoryCompressedTTF(icons_data, icons_size, scaled_font_size - 55.f, &icons_config, icons_ranges);
}

/*
 * Memory based head and body fonts wrapped in 1 function in order to operate fast
 */
void menu::widgets::head_text(const std::string& text, bool multi_line, bool disabled, bool centered) noexcept { wtools::text(font::head, text, multi_line, disabled, centered); }
void menu::widgets::body_text(const std::string& text, bool multi_line, bool disabled, bool centered) noexcept { wtools::text(font::body, text, multi_line, disabled, centered); }
void menu::widgets::foot_text(const std::string& text, bool multi_line, bool disabled, bool centered) noexcept { wtools::text(font::foot, text, multi_line, disabled, centered); }

/*
 * Buttons wrapped in their different fonts in order to use them without the problem
 * of pushing and popping the same fonts in a redundant form
 */
bool menu::widgets::head_button(const std::string &text, bool centered) noexcept { return wtools::button(font::head, text, centered); }
bool menu::widgets::body_button(const std::string &text, bool centered) noexcept { return wtools::button(font::body, text, centered); }
bool menu::widgets::foot_button(const std::string &text, bool centered) noexcept { return wtools::button(font::foot, text, centered); }

/*
 * Input text with 'font::body' by default because I really doubt it will be used with 'font::head' or 'font::foot' lol
 * PD: I really needed one now
 */
bool menu::widgets::input(char* text, std::size_t text_size, const std::string &hint, bool multi_line, const std::string& icon, ImGuiInputTextFlags iflags, bool centered) noexcept { return wtools::input(font::body, text, text_size, hint, multi_line, icon, iflags, centered); }
bool menu::widgets::input_foot(char* text, std::size_t text_size, const std::string &hint, bool multi_line, const std::string& icon, ImGuiInputTextFlags iflags, bool centered) noexcept { return wtools::input(font::foot, text, text_size, hint, multi_line, icon, iflags, centered); }

/*
 * Wrapped combo to be used, nothing else to comment to this
 */
const char* menu::widgets::combo(const char *label, const char *preview_value, std::vector<const char*> items,bool centered) noexcept { return wtools::combo(font::body, label, preview_value, items, centered); }


bool menu::widgets::checkbox(const char *label, bool *v, bool centered) noexcept { return wtools::checkbox(font::body, label, v, centered); }
/*
 * This represents the big title on every tab seen in Hard Motion.
 *
 * The BeginChild text will have a '###'.
 *
 * If I called this function 2 times or more it would mix things up.
 * But since I will use once per tab it's ok to have it like that
 */
void menu::widgets::upper_title(const std::string &text) noexcept
{
    ImGui::Dummy(ImVec2(0.f, 200.f)); // if logged in, less ( maybe / 2 ?? ) so it can fit the profile square

    window_with_margins("###", ImGui::CalcTextSize(text.c_str()).y + 10.f);

    head_text(text);

    ImGui::EndChild();
}

/*
 * Loads the Hard Motion logo
 */
void menu::widgets::logo() noexcept
{
    ImVec2 image_size(700.f, 700.f); //soon custom in order to add it wherever you want

    ImGui::Dummy(ImVec2(0.f, 200.f));

    wtools::image(hard_motion_logo, sizeof hard_motion_logo, image_size, true);

    ImGui::Dummy(ImVec2(0.f, 200.f));
}
/*
 * This function adds margins to the BeginChild's, It doesn't have 'ImGui::EndChild'!!!, after
 * calling this you're responsible of adding the correspondent EndChild to never get problems
 * and get nice margins B)
 */
bool menu::widgets::window_with_margins(const std::string &label, float vertical_length, float vertical_margin, ImVec4 col, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags) noexcept
{
    if (vertical_margin != 0.f)
        ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, vertical_margin));

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + scales::margin);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, col);

    bool used = ImGui::BeginChild(label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - scales::margin, vertical_length), child_flags, window_flags);

    ImGui::PopStyleColor();

    return used;
}

/*
 * Custom 'ImGui::EndChild()' to add a margin at the end!
 */
void menu::widgets::end_window_with_margins(float vertical_margin) noexcept
{
    ImGui::EndChild();

    if (vertical_margin != 0.f)
        ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, vertical_margin));
}

/*
 * a event, represented
 */
bool menu::widgets::event(const event_t event) noexcept
{
    std::string event_name = event.get_title();

    bool used = window_with_margins("###" + event_name, scales::event, scales::event_margin, colors::widgets);

    foot_text(event_name, false, false);

    foot_text(wtools::get_curiosity_text(event.get_info()), false, true);

    if (widgets::foot_button("info"))
    {
        values::current_event = event;
        core::go_to_tab(core::tab_t::_event_info);
    }

    end_window_with_margins(scales::event_margin);

    return used;
}

/*
 *
 * User searched on the user searcher
 */
bool menu::widgets::user(member_t member, core::tab_t destiny) noexcept
{
    std::string name = member.get_name();

    bool used = window_with_margins(name, scales::user,scales::user_margin, colors::widgets);

    foot_text(name, false, false);

    std::string dance = member.get_dance_type();

    foot_text(dance, false, true);

    if (widgets::foot_button("info"))
    {
        values::current_member = member;
        go_to_tab(destiny);
    }

    end_window_with_margins(scales::user_margin);

    return used;
}


/*
 * Used for those functions that doesn't have any default margin
 */
void menu::widgets::bulk(float more) noexcept { ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, scales::margin * more)); }

/*
 * Recycling input text, I made a lazy block info to show user and member info
 */
void menu::widgets::info_block(const std::string& info, const std::string& icon) noexcept
{
    char* info_cchar = strdup(info.c_str());

    widgets::input(info_cchar, strlen(info_cchar), "", false, icon, ImGuiInputTextFlags_ReadOnly);

    free(info_cchar); //deam
}

/*
 * This function will update the tabs visually.
 *
 * ImGui doesn't have a tab system so I may make it somehow from ground up..
 * By tab system I refer to a View system like Android Studio Interface System has
 *
 * Overall it seems good
 */
void menu::core::go_to_tab(tab_t tab) noexcept
{
    if (tab < _landing or tab > _admin_panel)
        return;

    switch ((current_tab = tab))
    {
        case _landing:  lobby::landing();                                    break;

        case _log_in:  lobby::auth::log_in();                                break;
        case _register_in:  lobby::auth::register_in();                      break;

        case _hub:  lobby::main::hub();                                      break;

        case _events:  lobby::main::events::events();                        break;
        case _event_info:  lobby::main::events::event_info();                break;
        case _event_members:  lobby::main::events::members::event_members(); break;
        case _member_info:  lobby::main::events::members::member_info();     break;
        case _create_event:  lobby::main::events::create_event();            break;
        case _joined_events:  lobby::main::events::joined_events();          break;

        case _search: lobby::main::search::search();                         break;
        case _user_info: lobby::main::search::user::user_info();             break;
        case _filter: lobby::main::search::filter();                         break;

        case _user: lobby::main::user::user();                               break;
        case _edit_user_info: lobby::main::user::edit_user_info();           break;
        default:                                                             break;
    }
}

/*
 * This function simulates what Android does as when you press
 * the back button. I call this on the JNICALL of 'native-lib.cpp', on
 * the callback 'onKeyPressed' of 'MainActivity'
 */
void menu::core::go_back() noexcept
{
    switch(current_tab)
    {
        case _landing:
            break;

        case _log_in:
        case _register_in:
            go_to_tab(_landing);
            break;

        case _hub: // retarded option kekekekekeeke
            //go_to_tab(_landing); when logged in and we want to log off after
            break;

        case _user:
        case _search:
        case _events:
            go_to_tab(_hub);
            break;

        case _event_info:
            go_to_tab(_events);
            break;
        case _event_members:
            go_to_tab(_event_info);
            break;

        case _member_info:
            go_to_tab(_event_members);
            break;

        case _create_event:
        case _joined_events:
            go_to_tab(_events);
            break;

        case _user_info:
        case _filter:
            go_to_tab(_search);
            break;

        case _edit_user_info:
            go_to_tab(_user);
            break;
    }
}

/*
 * Where you land first, the home page
 */
void menu::core::lobby::landing() noexcept
{
    widgets::upper_title("hard motion");

    widgets::logo();

    {
        widgets::window_with_margins("###options", scales::option * 2);

        if (widgets::body_button("login"))
            core::go_to_tab(tab_t::_log_in);

        if (widgets::body_button("register"))
            core::go_to_tab(tab_t::_register_in);

        widgets::end_window_with_margins();
    }
}

/*
 * This and register are same stuff, but I will keep the redundancy in this 2 ones in order to be maintainable
 */
void menu::core::lobby::auth::log_in() noexcept
{
    static char user_name[16] = {};
    static char pass_word[16] = {};

    static std::vector<const char*> fields = { user_name, pass_word };

    widgets::upper_title("user login");

    {
        widgets::window_with_margins("###log_in_panel", scales::input * 2, scales::margin * 10);

        widgets::input(user_name, sizeof user_name, "username..");
        widgets::input(pass_word, sizeof pass_word, "password..", false, "", ImGuiInputTextFlags_Password);

        widgets::end_window_with_margins(scales::margin * 9);
    }

    {
        widgets::window_with_margins("###options", scales::option);

        if (widgets::body_button("login"))
        {
            if (helper::validate_fields(fields))
            {
                auto result = api_rest_fetch::get_member_by_name(user_name);

                if (result.first && result.second.get_password() == pass_word)
                {
                    values::user_data::member_local = result.second;
                    values::user_data::logged_in = true;
                    helper::auto_remove(fields, true);
                    values::user_data::is_admin = values::user_data::member_local.get_name() == "tese"; // lol XDDDD
                    go_to_tab(tab_t::_hub);
                }
            }
        }

        widgets::end_window_with_margins();
    }

    helper::auto_remove(fields);
}


/*
 * This and login are same stuff, but I will keep the redundancy in this 2 ones in order to be maintainable
 */
void menu::core::lobby::auth::register_in() noexcept
{
    static char user_name[16] = {};
    static char pass_word[16] = {};
    static char number[10] = {};
    static char email[32] = {};
    const char* dance;

    static std::vector<const char*> fields = { user_name, pass_word, number, email };

    widgets::upper_title("user register");

    {
        widgets::window_with_margins("###register_in_panel", (scales::input * 4) + scales::combo,
                                     scales::margin * 4);

        widgets::input(user_name, sizeof user_name, "username..");
        widgets::input(pass_word, sizeof pass_word, "password..", false, "", ImGuiInputTextFlags_Password);
        widgets::input(number, sizeof number, "number..");
        widgets::input(email, sizeof email, "e-mail..");

        static const char *current_value = "type of dance";
        current_value = dance = widgets::combo("###dances", current_value, values::dances);

        widgets::end_window_with_margins(scales::margin * 4);
    }

    {
        widgets::window_with_margins("###options", scales::option);

        if (widgets::body_button("register"))
        {
            member_t member_to_register(api_rest_fetch::get_latest_id_to_give("members"), user_name, pass_word, number, email, dance);

            if (helper::validate_fields(fields) && api_rest_fetch::post_member(member_to_register))
            {
                values::user_data::member_local = member_to_register;
                helper::auto_remove(fields, true);
                go_to_tab(tab_t::_hub);
            }
        }

        widgets::end_window_with_margins();
    }

    helper::auto_remove(fields);
}


void menu::core::lobby::main::hub() noexcept
{
    widgets::upper_title("hard motion");

    {
        widgets::window_with_margins("###hub", scales::option * 3, scales::margin * 7);

        if (widgets::body_button("events"))
            go_to_tab(tab_t::_events);

        if (widgets::body_button("search"))
            go_to_tab(tab_t::_search);

        if (widgets::body_button("user"))
            go_to_tab(tab_t::_user);

        widgets::end_window_with_margins();
    }
}

void menu::core::lobby::main::events::events() noexcept
{
    values::user_data::joined_already = false;

    static char search_event[32] = {};

    widgets::upper_title("events");

    {
        widgets::window_with_margins("###search_event", scales::input, scales::margin_before_title);

        widgets::input(search_event, sizeof search_event, "event..", false, ICON_FA_SEARCH);

        widgets::end_window_with_margins(scales::slight_space_between_widgets);
    }

    {
        widgets::window_with_margins("###events", scales::option * 4,
                                     scales::slight_space_between_widgets, colors::child,
                                     ImGuiChildFlags_None,
                                     ImGuiWindowFlags_AlwaysVerticalScrollbar);

        if (values::refresh_events)
        {
            values::current_events = api_rest_fetch::get_events();
            values::refresh_events = false;
        }

        for (event_t event: values::current_events)
        {
            std::vector<std::string> members_of_the_event = event.get_members();
            auto it = std::find(members_of_the_event.begin(), members_of_the_event.end(), values::user_data::member_local.get_id());

            if (helper::lower(event.get_title()).find(search_event) != std::string::npos && it == members_of_the_event.end())
                widgets::event(event);
        }

        widgets::end_window_with_margins(scales::slight_space_between_widgets);
    }

    {
        widgets::window_with_margins("###events_options", scales::option * 2,
                                     scales::slight_space_between_widgets);

        if (widgets::body_button("create"))
        {
            values::refresh_events = true;
            go_to_tab(tab_t::_create_event);
        }

        if (widgets::body_button("joined"))
        {
            values::refresh_events = true;
            values::current_events = api_rest_fetch::get_events_from_member(values::user_data::member_local.get_id());
            go_to_tab(tab_t::_joined_events);
        }

        widgets::end_window_with_margins();
    }

    if (change_tab)
        values::refresh_events = true;
}

void menu::core::lobby::main::events::event_info() noexcept
{
    bool admin = values::user_data::is_admin;
    bool joined = values::user_data::joined_already;

    widgets::upper_title("event info");

    float body_size_y_with_pad = values::get_font_size(font::body).y + 10.f;
    {
        widgets::window_with_margins("###event_title", body_size_y_with_pad, admin ? scales::margin : scales::margin_before_title);

        widgets::body_text(values::current_event.get_title());

        widgets::end_window_with_margins(scales::slight_space_between_widgets);
    }

    {
        widgets::window_with_margins("###event_info_with_description",
                                     scales::option + (scales::event * 2) / 1.2,
                                     scales::slight_space_between_widgets);

        wtools::align();
        widgets::foot_text(values::current_event.get_info(), true, false, false);

        widgets::end_window_with_margins(scales::slight_space_between_widgets);
    }

    {
        int option_count = 2 + (admin ? 1 : 0);

        widgets::window_with_margins("###members", scales::option * (joined ? option_count - 1 : option_count)); // this is retard but idk it works

        std::string current_event_id = values::current_event.get_id();

        if (widgets::body_button("members"))
        {
            values::current_members = api_rest_fetch::get_members_from_event(current_event_id);
            go_to_tab(tab_t::_event_members);
        }

        if (!joined)
            if (widgets::body_button("join"))
                if (api_rest_fetch::join_member_to_event(values::user_data::member_local.get_id(), current_event_id))
                {
                    values::refresh_events = true;
                    go_back();
                }

        if (admin)
            if (widgets::body_button("delete"))
            {
                values::refresh_events = true;
                api_rest_fetch::delete_event(current_event_id);
                go_back();
            }

        widgets::end_window_with_margins();
    }
}

void menu::core::lobby::main::events::members::event_members() noexcept
{
    static char search_member[32] = {};

    static std::vector<const char*> fields = { search_member };

    widgets::upper_title("members");

    {
        widgets::window_with_margins("###members_search", scales::input,
                                     scales::margin_before_title);
        widgets::input(search_member, sizeof search_member, "member..", false, ICON_FA_SEARCH);
        widgets::end_window_with_margins(scales::event_margin);
    }

    {
        widgets::window_with_margins("###members_names", scales::option * 6,
                                     scales::slight_space_between_widgets);

        for (const auto& member : values::current_members)
        {
            const auto& member_name = helper::lower(member.get_name());
            const auto& local_member_name = helper::lower(values::user_data::member_local.get_name());

            if (member_name.find(search_member) != std::string::npos && member_name.find(local_member_name) == std::string::npos)
                widgets::user(member, tab_t::_member_info);
        }

        widgets::end_window_with_margins();
    }

    helper::auto_remove(fields);
}

void menu::core::lobby::main::events::members::member_info() noexcept
{
    widgets::upper_title("member info");

    {
        widgets::window_with_margins("###member_info", scales::input * 3,
                                     scales::margin_before_title);

        widgets::info_block(values::current_member.get_name());
        widgets::info_block(values::current_member.get_number());
        widgets::info_block(values::current_member.get_dance_type());

        widgets::end_window_with_margins(scales::margin * 4);
    }

    {
        widgets::window_with_margins("###member_deletion", scales::option, scales::margin_before_title);

        if (widgets::body_button("delete"))
            if (api_rest_fetch::delete_member_by_name(values::current_member.get_name()))
            {
                values::refresh_members = true;
                go_back();
            }

        widgets::end_window_with_margins();
    }
}

void menu::core::lobby::main::events::create_event() noexcept
{
    static char event_title[32]  = {};
    static char event_info[255]  = {};

    static std::vector<const char*> fields = { event_title, event_info };

    float default_scale = scales::margin_before_title;

    widgets::upper_title("create event");

    {
        widgets::window_with_margins("###create_event", scales::input + (scales::input * 4.13),
                                     default_scale);

        widgets::input(event_title, sizeof event_title, "event title..");
        widgets::input_foot(event_info, sizeof event_info, "",
                            true); //Multiline doesn't support hint

        widgets::end_window_with_margins(scales::margin * 6);
    }

    {
        widgets::window_with_margins("###upload", scales::option);

        if (widgets::body_button("upload"))
            if (helper::validate_fields({event_title, event_info}))
                if (api_rest_fetch::post_event(event_t(std::string(event_title), std::string(event_info))))
                {
                    values::refresh_members = true;
                    helper::erase_array_data(event_title);
                    helper::erase_array_data(event_info);
                    go_to_tab(tab_t::_events);
                }

        widgets::end_window_with_margins();
    }

    helper::auto_remove(fields);
}

void menu::core::lobby::main::events::joined_events() noexcept
{
    static char search_joined_event[32] = {};

    static std::vector<const char*> fields = { search_joined_event };

    widgets::upper_title("joined events");

    {
        widgets::window_with_margins("###search_event", scales::input, scales::margin_before_title);

        widgets::input(search_joined_event, sizeof search_joined_event, " joined event..", false,
                       ICON_FA_SEARCH);

        widgets::end_window_with_margins(scales::slight_space_between_widgets);
    }

    {
        widgets::window_with_margins("###joined_events", scales::option * 6,
                                     scales::slight_space_between_widgets, colors::child,
                                     ImGuiChildFlags_None,
                                     ImGuiWindowFlags_AlwaysVerticalScrollbar);

        for (event_t event: values::current_events)
            if (helper::lower(event.get_title()).find(search_joined_event) != std::string::npos)
            {
                values::user_data::joined_already = true;
                widgets::event(event);
            }

        widgets::end_window_with_margins();
    }

    helper::auto_remove(fields);
}

void menu::core::lobby::main::search::search() noexcept
{
    widgets::upper_title("search");

    static char search_user[32] = {};

    {
        widgets::window_with_margins("###search_user", scales::input, scales::margin_before_title);

        widgets::input(search_user, sizeof search_user, "user..", false, ICON_FA_SEARCH);

        widgets::end_window_with_margins(scales::slight_space_between_widgets);
    }

    {
        widgets::window_with_margins("###users", scales::option * 4,
                                     scales::slight_space_between_widgets, colors::child,
                                     ImGuiChildFlags_None,
                                     ImGuiWindowFlags_AlwaysVerticalScrollbar);

        if (values::refresh_members)
        {
            values::current_members = api_rest_fetch::get_members();
            values::refresh_members = false;
        }

        for (const auto& member : values::current_members)
        {
            const auto& member_name = helper::lower(member.get_name());
            const auto& local_member_name = helper::lower(values::user_data::member_local.get_name());

            if (member_name.find(search_user) != std::string::npos && member_name.find(local_member_name) == std::string::npos && helper::check_filter(member.get_dance_type()))
                widgets::user(member, tab_t::_user_info);
        }

        widgets::end_window_with_margins(scales::slight_space_between_widgets);
    }

    {
        widgets::window_with_margins("###search_options", scales::option, scales::margin * 4);

        if (widgets::body_button("filter"))
        {
            values::refresh_members = true;
            go_to_tab(tab_t::_filter);
        }

        widgets::end_window_with_margins();
    }
}


void menu::core::lobby::main::search::user::user_info() noexcept
{
    widgets::upper_title("user info");

    {
        widgets::window_with_margins("###user_info", scales::input * 3, scales::margin * 7);

        widgets::info_block(values::current_member.get_name());
        widgets::info_block(values::current_member.get_number());
        widgets::info_block(values::current_member.get_dance_type());

        widgets::end_window_with_margins(scales::margin * 4);
    }

    {
        widgets::window_with_margins("###admin_delete", scales::option,scales::margin_before_title);

        if (widgets::body_button("delete"))
            if (api_rest_fetch::delete_member_by_name(values::current_member.get_name()))
            {
                values::refresh_members = true;
                go_back();
            }

        widgets::end_window_with_margins();
    }
}

void menu::core::lobby::main::search::filter() noexcept
{
    /*
     * ported from my old kappa ImGui project
     */
    auto handled_checkbox = [&](const char* label, bool& flag)
    {
        if (widgets::checkbox(label, &flag, false))
        {
            if (flag)
            {
                values::jumpstyle_filter = false;
                values::hakken_filter = false;
                values::shuffle_filter = false;
                flag = true;
            }
            else flag = false;
        }
    };

    widgets::upper_title("filter");

    {
        widgets::window_with_margins("###filter_options", scales::option * 2, scales::margin * 7);

        wtools::align();
        handled_checkbox("jumpstyle", values::jumpstyle_filter);

        wtools::align();
        handled_checkbox("hakken", values::hakken_filter);

        wtools::align();
        handled_checkbox("shuffle", values::shuffle_filter);

        widgets::end_window_with_margins();
    }
}

void menu::core::lobby::main::user::user() noexcept
{
    widgets::upper_title("user");

    {
        widgets::window_with_margins("###user_info_local", scales::info * 4,
                                     scales::margin_before_title);

        widgets::info_block(values::user_data::member_local.get_name());
        widgets::info_block(values::user_data::member_local.get_e_mail());
        widgets::info_block(values::user_data::member_local.get_number());
        widgets::info_block(values::user_data::member_local.get_dance_type());

        widgets::end_window_with_margins(scales::margin * 4);
    }

    {
        widgets::window_with_margins("###edit_info", scales::option * 2);

        if (widgets::body_button("edit"))
            go_to_tab(tab_t::_edit_user_info);

        if (widgets::body_button("log off"))
        {
            values::user_data::is_admin = false;
            go_to_tab(tab_t::_landing);
        }

        widgets::end_window_with_margins();
    }
}

void menu::core::lobby::main::user::edit_user_info() noexcept
{
    widgets::upper_title("edit");

    static char email[32] = {};
    static char number[10] = {};
    const char* dance;

    static std::vector<const char*> fields = { email, number };

    helper::auto_remove(fields);

    static bool add_once = false;
    static const char *current_value;

    if (!add_once)
    {
        strcpy(email, values::user_data::member_local.get_e_mail().c_str());
        strcpy(number, values::user_data::member_local.get_number().c_str());
        current_value = values::user_data::member_local.get_dance_type().c_str();
        add_once = true;
    }

    {
        widgets::window_with_margins("###edit_panel", scales::info * 3,
                                     scales::margin_before_title);


        widgets::input(number, sizeof number, "number..");
        widgets::input(email, sizeof email, "e-mail..");

        current_value = dance = widgets::combo("###dances", current_value, values::dances);

        widgets::end_window_with_margins(scales::margin * 4);
    }

    {
        widgets::window_with_margins("###edit_info", scales::option);

        if (widgets::body_button("apply"))
        {
            json j;

            j["number"] = number;
            j["email"] = email;
            j["dance_type"] = dance;

            if (api_rest_fetch::update_member(values::user_data::member_local.get_id(), j))
            {
                add_once = false;
                values::user_data::member_local.set_number(number);
                values::user_data::member_local.set_email(email);
                values::user_data::member_local.set_dance_type(dance);
                go_back();
            }
        }

        if (change_tab)
            add_once = false;

        widgets::end_window_with_margins();
    }
}

ImVec2 menu::values::get_font_size(ImFont *font) noexcept
{
    ImGui::PushFont(font);
    ImVec2 size = ImGui::CalcTextSize("X");
    ImGui::PopFont();

    return size;
}

void menu::helper::erase_array_data(char *arr) noexcept { if (arr) arr[0] = '\0'; }

bool menu::helper::validate_field(const char *field) noexcept {return std::strlen(field) > 0;}

bool menu::helper::validate_fields(std::vector<const char*> fields) noexcept
{
    for (auto field : fields)
        if (!validate_field(field))
            return false;

    return true;
}

void menu::helper::auto_remove(std::vector<const char *> fields, bool auto_) noexcept
{
    bool what_to_react = !auto_ ? menu::core::change_tab : auto_;

    if (what_to_react)
        for (auto field : fields)
            erase_array_data(const_cast<char*>(field));
}

std::string menu::helper::case_(const std::string& str, std::function<int(int)> f) noexcept
{
    std::string _str;

    for (int x = 0; x < str.length(); x++)
        _str += f(str[x]);

    return _str;
}

std::string menu::helper::lower(const std::string& str) noexcept { return case_(str, tolower); }

std::string menu::helper::upper(const std::string& str) noexcept { return case_(str, toupper); }

bool menu::helper::check_filter(const std::string& dance_to_check) noexcept
{
    std::map<bool, std::string> dances_active = { {values::jumpstyle_filter, values::dances[0]}, {values::hakken_filter, values::dances[1]}, {values::shuffle_filter, values::dances[2]} };

    for (auto const & [active, dance_name] : dances_active)
        if (active && dance_name.find(dance_to_check) == std::string::npos)
            return false;

    return true;
}
