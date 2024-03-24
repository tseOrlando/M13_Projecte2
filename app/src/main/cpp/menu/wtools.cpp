//
// Created by sel on 24/03/2024.
//

#include "main/cpp/menu/headers/menu.h"

/*
 * Changes the font of the text, in my case it's just resizing so that's why
 * it's called 'resize_text', only used for 'head_text' and 'body_text'
 */
void wtools::resize_text(ImFont* font, const std::string& text) noexcept
{
    ImGui::PushFont(font);
    ImGui::Text("%s", text.c_str());
    ImGui::PopFont();
}

/*
 * Centers whatever is called ( as long as is a ImGui widget ) below of the call of 'center'
 */
void wtools::center() noexcept { ImGui::SetCursorPosX((ImGui::GetWindowSize().x / 2) * 0.5f); }
