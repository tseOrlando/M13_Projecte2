//
// Created by sel on 21/03/2024.
//

#include "../menu/headers/menu.h"

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
    ImGui::SetNextWindowPos(ImVec2(-5, -5));
    ImGui::SetNextWindowSize(ImVec2(display_size.x + 10, display_size.y + 10));

    ImGui::Begin("Hard Motion", nullptr, flags);

    ImGui::Text("HARD MOTION");

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
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = static_cast<int>(2.5);
    icons_config.OversampleV = static_cast<int>(2.5);

    ImVec2 display_size = ImGui::GetIO().DisplaySize;

    float scaled_font_size = menu::size::scale; // base

    scaled_font_size *= (((display_size.x / display_size.y) < 1.5) ? 0.8f : 1.2f);

    static const unsigned short icons_ranges[] = { 0xe005, 0xf8ff, 0 };

    menu::font::head = io.Fonts->AddFontFromMemoryTTF(&man_rope, sizeof man_rope, scaled_font_size + 20);
    io.Fonts->AddFontFromMemoryCompressedTTF(icons_data, icons_size, scaled_font_size - 15, &icons_config, icons_ranges);

    menu::font::body = io.Fonts->AddFontFromMemoryTTF(&man_rope, sizeof man_rope, scaled_font_size);
    io.Fonts->AddFontFromMemoryCompressedTTF(icons_data, icons_size, scaled_font_size - 35, &icons_config, icons_ranges);

    ImGui::GetStyle().ScaleAllSizes(3.0f);
}