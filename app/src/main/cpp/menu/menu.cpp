//
// Created by sel on 21/03/2024.
//

#include "../menu/headers/menu.h"
#include "headers/memory_images/hard_motion_logo.h"

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

    core::update_tab(14);

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

    float scaled_font_size = menu::size::scale; // base

    scaled_font_size *= (((display_size.x / display_size.y) < 1.5) ? 0.8f : 1.2f);

    static const unsigned short icons_ranges[] = { 0xe005, 0xf8ff, 0 };

    menu::font::head = io.Fonts->AddFontFromMemoryTTF(&man_rope, sizeof man_rope, scaled_font_size + 100.f);
    io.Fonts->AddFontFromMemoryCompressedTTF(icons_data, icons_size, scaled_font_size - 15.f, &icons_config, icons_ranges);

    menu::font::body = io.Fonts->AddFontFromMemoryTTF(&man_rope, sizeof man_rope, scaled_font_size + 20.f);
    io.Fonts->AddFontFromMemoryCompressedTTF(icons_data, icons_size, scaled_font_size - 35.f, &icons_config, icons_ranges);
}

/*
 * Memory based head and body fonts wrapped in 1 function in order to operate fast
 */
void menu::widgets::head_text(const std::string& text, bool centered) noexcept { wtools::text(font::head, text, centered); }
void menu::widgets::body_text(const std::string& text, bool centered) noexcept { wtools::text(font::body, text, centered); }

/*
 * Buttons wrapped in their different fonts in order to use them without the problem
 * of pushing and popping the same fonts in a redundant form
 */
bool menu::widgets::head_button(const std::string &text, bool centered) noexcept { return wtools::button(font::head, text, centered); }
bool menu::widgets::body_button(const std::string &text, bool centered) noexcept { return wtools::button(font::body, text, centered); }

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
bool menu::widgets::window_with_margins(const std::string &label, float vertical_length, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags) noexcept
{
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + scales::margin);

    return ImGui::BeginChild(label.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - scales::margin, vertical_length), child_flags, window_flags);
}

/*
 * This function will update the tabs internally and visually at the same time.
 *
 * ImGui doesn't have a tab system so I may make it somehow from ground up..
 * By tab system I refer to a View system like Android Studio Interface System has
 *
 * Overall it seems good
 */
void menu::core::update_tab(int tab)
{
    switch (tab)
    {
        case 0:  lobby::landing();                               break;

        case 1:  lobby::auth::log_in();                          break;
        case 2:  lobby::auth::register_in();                     break;

        case 3:  lobby::main::hub();                             break;

        case 4:  lobby::main::events::events();                  break;
        case 5:  lobby::main::events::event_info();              break;
        case 6:  lobby::main::events::members::event_members();  break;
        case 7:  lobby::main::events::members::member_info();    break;
        case 8:  lobby::main::events::create_event();            break;
        case 9:  lobby::main::events::joined_events();           break;

        case 10: lobby::main::search::search();                  break;
        case 11: lobby::main::search::filter();                  break;

        case 12: lobby::main::user::user();                      break;
        case 13: lobby::main::user::edit_user_info();            break;
        case 14: lobby::main::user::admin_panel();               break;
    }
}

/*
 * Where you land first, the home page
 */
void menu::core::lobby::landing() noexcept
{
    widgets::upper_title("hard motion");

    widgets::logo();

    widgets::window_with_margins("###options", scales::option * 2, ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);

    if (widgets::body_button("login"))
    {

    }

    if (widgets::body_button("register"))
    {

    }

    ImGui::EndChild();
}

void menu::core::lobby::auth::log_in() noexcept
{
    widgets::upper_title("user login");
}

void menu::core::lobby::auth::register_in() noexcept
{
    widgets::upper_title("user register");
}

void menu::core::lobby::main::hub() noexcept
{
    widgets::upper_title("hard motion");
}

void menu::core::lobby::main::events::events() noexcept
{
    widgets::upper_title("events");
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


