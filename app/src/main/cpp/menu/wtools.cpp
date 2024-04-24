//
// Created by sel on 24/03/2024.
//

#include "main/cpp/menu/headers/menu.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"
#include "main/cpp/menu/headers/wtools/wtools.h"


/*
 * This function is all-in-one for text transformation
 * You can :
 * - Change the font
 * - Center it
 */
void wtools::text(ImFont* font, const std::string& text, bool multi_line, bool disabled, bool centered) noexcept
{
    ImGui::PushFont(font);
    if (centered)
        ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(text.c_str()).x) * 0.5f);

    if (!disabled && !multi_line) ImGui::Text("%s", text.c_str());
    else if (!multi_line) ImGui::TextDisabled("%s", text.c_str());
    else ImGui::TextWrapped("%s", text.c_str());
    ImGui::PopFont();
}

/*
 * This function is all-in-one for button transformation
 * You can :
 * - Change the font
 * - Center it
 */
bool wtools::button(ImFont *font, const std::string &text, bool centered) noexcept
{
    bool used = false;

    float margin = menu::scales::margin / 2;

    ImGui::Dummy(ImVec2(0, margin));

    if (centered)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + margin);

    ImGui::PushFont(font);

    used = ImGui::Button(text.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - margin, 0));

    ImGui::PopFont();

    ImGui::Dummy(ImVec2(0, margin / 4));

    return used;
}

/*
 * Loads an image from memory with OpenGL.
 * First loading it from the memory and uploading it to the GPU
 * binding and using ImGui API to load it into the UI
 */
void wtools::image(const unsigned char* data, size_t data_size, ImVec2 size, bool centered) noexcept
{
    int image_width, image_height, image_channels;
    unsigned char* image_data = stbi_load_from_memory(data, data_size, &image_width, &image_height, &image_channels, 4);

    if (image_data != nullptr)
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image_data);

        glBindTexture(GL_TEXTURE_2D, texture);

        if (centered)
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - size.x) * 0.5f);

        return ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture)), size);
    }

    return;
}

/*
 * This function is all-in-one for input transformation
 * You can :
 * - Change the font
 * - Center it
 */
bool wtools::input(ImFont *font, char* text, std::size_t text_size,  const std::string &hint, bool multi_line, const std::string& icon, ImGuiInputTextFlags flags, bool centered) noexcept
{
    bool used = false;

    float margin = menu::scales::margin / 2;

    ImGui::Dummy(ImVec2(0, margin));

    if (centered)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + margin);

    ImGui::PushFont(font);

    if (icon != "") //xd
    {
        ImGui::Text("%s", icon.c_str());
        ImGui::SameLine(114, -10);
    }

    /*
     * Hint always different, label different
     */
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - margin);

    if (!multi_line)
        used = ImGui::InputTextWithHint(std::string("###" + hint).c_str(), hint.c_str(), text, text_size, flags);
    else used = ImGui::InputTextMultiline(std::string("###" + hint).c_str(), text, text_size);
    ImGui::PopItemWidth();

    ImGui::PopFont();

    ImGui::Dummy(ImVec2(0, margin / 4));

    return used;
}

const char* wtools::combo(ImFont* font, const char *label, const char* preview_value, std::vector<const char*> items, bool centered) noexcept
{
    const char* selected_item = preview_value;

    float margin = menu::scales::margin / 2;

    ImGui::Dummy(ImVec2(0, margin));

    if (centered)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + margin);

    ImGui::PushFont(font);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - margin);
    if (ImGui::BeginCombo(label, preview_value))
    {
        for (const char* item : items)
            if (ImGui::Selectable(item))
            {
                selected_item = item;
                preview_value = item;
            }

        ImGui::EndCombo();
    }

    ImGui::PopItemWidth();
    ImGui::PopFont();

    ImGui::Dummy(ImVec2(0, margin / 4));

    return selected_item;
}

/*
 * Tries to make the user curiosity by cropping the text in half and adding 3 dots
 */
std::string wtools::get_curiosity_text(const std::string &text) noexcept
{
    float text_size_x = ImGui::CalcTextSize(text.c_str()).x;

    return (text_size_x > ImGui::GetContentRegionAvail().x ? text.substr(0, static_cast<int>(text.length() / 1.5)) + "..." : text);
}
/*
 * This function just aligns to the margin the menu has as a global
 * value so it's hardcoded, just to get the control of what the function
 * can do and add more features to this as well if needed
 */
void wtools::align() noexcept { ImGui::SetCursorPosX(ImGui::GetCursorPosX() + menu::scales::margin); }
