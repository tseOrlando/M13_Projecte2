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
void menu::widgets::head_text(const std::string& text, bool multi_line, bool centered) noexcept { wtools::text(font::head, text, multi_line, centered); }
void menu::widgets::body_text(const std::string& text, bool multi_line, bool centered) noexcept { wtools::text(font::body, text, multi_line, centered); }

/*
 * Buttons wrapped in their different fonts in order to use them without the problem
 * of pushing and popping the same fonts in a redundant form
 */
bool menu::widgets::head_button(const std::string &text, bool centered) noexcept { return wtools::button(font::head, text, centered); }
bool menu::widgets::body_button(const std::string &text, bool centered) noexcept { return wtools::button(font::body, text, centered); }

/*
 * Input text with 'font::foot' by default because I really doubt it will be used with 'font::head' or 'font::body' lol
 */
bool menu::widgets::input(char* text, std::size_t text_size, const std::string &hint, ImGuiInputTextFlags iflags, bool centered) noexcept { return wtools::input(font::body, text, text_size, hint, iflags, centered); }

/*
 * Wrapped combo to be used, nothing else to comment to this
 */
const char* menu::widgets::combo(const char *label, const char *preview_value, std::vector<const char*> items,bool centered) noexcept { return wtools::combo(font::body, label, preview_value, items, centered); }

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
bool menu::widgets::window_with_margins(const std::string &label, float vertical_length, float vertical_margin, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags) noexcept
{
    if (vertical_margin != 0.f)
        ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, vertical_margin));

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + scales::margin);

    return ImGui::BeginChild(label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - scales::margin, vertical_length), child_flags, window_flags);
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
 * This function is used to display event data, the void func is the function
 * that will be called when clicked the info button inside of it, you will need
 * to make a lambda function outside of this function call in order to add it on
 * the parameters
 */
bool menu::widgets::window_surface_info(const std::string &label, const std::string& info, std::function<void(const std::string& title, const std::string& content)> post_call_func, float vertical_length, float vertical_margin) noexcept
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, colors::widgets);

    bool used = window_with_margins("###" + label, vertical_length, vertical_margin);

    ImGui::PopStyleColor();

    widgets::body_text(label);

    widgets::body_text(info, true, false);

    if (widgets::body_button("info"))
        post_call_func(label, info);

    end_window_with_margins(vertical_margin);

    return used;
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
    if (tab < _landing or tab > _user)
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
        case _filter: lobby::main::search::filter();                         break;

        case _user: lobby::main::user::user();                               break;
        case _edit_user_info: lobby::main::user::edit_user_info();           break;
        case _admin_panel: lobby::main::user::admin_panel();                 break;
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

        case _hub:
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

        case _filter:
            go_to_tab(_search);
            break;

        case _edit_user_info:
        case _admin_panel:
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

    widgets::window_with_margins("###options", scales::option * 2);

    if (widgets::body_button("login"))
        core::go_to_tab(tab_t::_log_in);

    if (widgets::body_button("register"))
        core::go_to_tab(tab_t::_register_in);

    widgets::end_window_with_margins();
}

/*
 * This and register are same stuff, but I will keep the redundancy in this 2 ones in order to be maintainable
 */
void menu::core::lobby::auth::log_in() noexcept
{
    widgets::upper_title("user login");

    widgets::window_with_margins("###log_in_panel", scales::input * 2, scales::margin * 10);

    widgets::input(values::user_data::user_name, sizeof values::user_data::user_name, "username..");
    widgets::input(values::user_data::pass_word, sizeof values::user_data::pass_word, "password..", ImGuiInputTextFlags_Password);

    widgets::end_window_with_margins(scales::margin * 9);

    widgets::window_with_margins("###options", scales::option);

    if (widgets::body_button("login"))
    {
        //-.- check
        go_to_tab(tab_t::_hub);
    }

    widgets::end_window_with_margins();
}

/*
 * This and login are same stuff, but I will keep the redundancy in this 2 ones in order to be maintainable
 */
void menu::core::lobby::auth::register_in() noexcept
{
    widgets::upper_title("user register");

    widgets::window_with_margins("###register_in_panel", (scales::input * 4) + scales::combo, scales::margin * 4);

    widgets::input(values::user_data::user_name, sizeof values::user_data::user_name, "username..");
    widgets::input(values::user_data::pass_word, sizeof values::user_data::pass_word, "password..", ImGuiInputTextFlags_Password);
    widgets::input(values::user_data::e_mail, sizeof values::user_data::e_mail, "e-mail..");
    widgets::input(values::user_data::number, sizeof values::user_data::number, "number..");

    /*
     * kek
     */
    static const char* current_value = "type of dance";
    current_value = values::user_data::dance = widgets::combo("###dances", current_value, values::dances);

    widgets::end_window_with_margins(scales::margin * 4);

    widgets::window_with_margins("###options", scales::option);

    if (widgets::body_button("register"))
    {
        //-.- check
        go_to_tab(tab_t::_hub);
    }

    widgets::end_window_with_margins();
}

void menu::core::lobby::main::hub() noexcept
{
    widgets::upper_title("hard motion");

    widgets::window_with_margins("###hub", scales::option * 3, scales::margin * 7);

    if (widgets::body_button("events"))
        go_to_tab(tab_t::_events);

    if (widgets::body_button("search"))
        go_to_tab(tab_t::_search);

    if (widgets::body_button("user"))
        go_to_tab(tab_t::_user);

    widgets::end_window_with_margins();
}

void menu::core::lobby::main::events::events() noexcept
{
    widgets::upper_title("events");

    widgets::window_with_margins("###search_event", scales::input, scales::margin * 3);

    widgets::input(values::search, sizeof values::search, "event..");

    widgets::end_window_with_margins(scales::margin);

    widgets::window_with_margins("###events", scales::option * 4, scales::margin, ImGuiChildFlags_None, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    /*
     * sample data
     */
    for (int i = 0; i <= 10; i++)
    {
        auto data = [](const std::string& title, const std::string& content)
        {
            //management
        };

        std::string event = "event nº " + std::to_string(i);
        std::string info  = "event info .. text lorem ipsum";

        widgets::window_surface_info(event, info, data, scales::event, scales::margin);
    }

    widgets::end_window_with_margins(scales::margin);

    widgets::window_with_margins("###events_options", scales::option, scales::margin * 3);

    if (widgets::body_button("create"))
        go_to_tab(tab_t::_create_event);

    ImGui::SameLine();

    if (widgets::body_button("joined", false))
        go_to_tab(tab_t::_joined_events);

    widgets::end_window_with_margins(scales::margin);
}

void menu::core::lobby::main::events::event_info() noexcept
{
    widgets::upper_title("event info");
}

void menu::core::lobby::main::events::members::event_members() noexcept
{
    widgets::upper_title("members");
}

void menu::core::lobby::main::events::members::member_info() noexcept
{
    widgets::upper_title("member info");
}

void menu::core::lobby::main::events::create_event() noexcept
{
    widgets::upper_title("create event");
}

void menu::core::lobby::main::events::joined_events() noexcept
{
    widgets::upper_title("joined events");
}

void menu::core::lobby::main::search::search() noexcept
{
    widgets::upper_title("search");
}

void menu::core::lobby::main::search::filter() noexcept
{
    widgets::upper_title("filter");
}

void menu::core::lobby::main::user::user() noexcept
{
    widgets::upper_title("user");
}

void menu::core::lobby::main::user::edit_user_info() noexcept
{
    widgets::upper_title("edit");
}

void menu::core::lobby::main::user::admin_panel() noexcept
{
    widgets::upper_title("admin panel");
}