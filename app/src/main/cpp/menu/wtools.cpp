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
 * The y parameter makes it available to align to a certain y-axis offset
 * The x parameter depends on the widget size meaning you still have to calculate the total
 * x value in order to the item be correctly aligned
 */
void wtools::center(float x, float y) noexcept { ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - x) * 0.5f, y)); }


