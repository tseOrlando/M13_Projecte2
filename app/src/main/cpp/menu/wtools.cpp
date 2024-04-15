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
void wtools::text(ImFont* font, const std::string& text, bool disabled, bool centered) noexcept
{
    if (centered)
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text.c_str()).x) * 0.5f);

    ImGui::PushFont(font);
    if (!disabled) ImGui::Text("%s", text.c_str());
    else ImGui::TextDisabled("%s", text.c_str());
    ImGui::PopFont();
}

/*
 * Centers wrapped text in order to have centered wrapped text, a thing ImGui
 * doesn't have by default
 *
 * PD: This ain't mine, it's from : 'https://github.com/ocornut/imgui/issues/4465'
 * I just took out some external functions and added them as lambdas
 */
void wtools::wrapped_text(ImFont* font, std::string text, ImVec2 position, int boundary_width, int lines, int y_padding) noexcept
{
    auto str_to_vec = [](std::string str, std::string token)
    {
        std::vector<std::string> result;
        while (str.size())
        {
            int index = str.find(token);
            if (index != std::string::npos)
            {
                result.push_back(str.substr(0, index));
                str = str.substr(index + token.size());
                if (str.size() == 0)
                    result.push_back(str);
            }
            else
            {
                result.push_back(str);
                str = "";
            }
        }
        return result;
    };

    auto dc_text = [](ImVec2 position, ImVec2 boundary, ImVec2 char_size, int idx, std::string text, int y_padding = 3)
    {
        ImVec2 render_pos;
        ImVec2 line_size = ImGui::CalcTextSize(text.c_str());
        render_pos.x = position.x + ((boundary.x / 2) - ((line_size.x + char_size.x) / 2)); // horizontal position centered in boundary x
        render_pos.y = position.y + ((idx * line_size.y) + y_padding);
        ImGui::RenderText(render_pos, text.c_str(), 0, false);
    };

    ImGui::PushFont(font);
    ImVec2 char_size = ImGui::CalcTextSize("X");
    ImGui::PopFont();

    std::vector<std::string> words = str_to_vec(text, " ");

    int line_cnt = 0; // 0 is first line
    int word_cnt = 0; // 0 is first word / vector item
    int line_x = 0;   // space already used on line
    std::string line_text = "";

    for (auto &word : words)
    {
        word_cnt++;
        int word_size = (word.length() + 1) * char_size.x; // size of this word in pixels, plus one space

        if ((line_x + word_size < boundary_width)) // if word will fit in space available
        {
            line_text += word + " "; // add it
            line_x += word_size;     // reduce the available space
            if (word_cnt == words.size()) // last one
            {
                dc_text(position, ImVec2(boundary_width, boundary_width), char_size, line_cnt, line_text.c_str());
                continue;
            }
        }

        else // word won't fit on this line, so draw current line, then start next line...
        {
            dc_text(position, ImVec2(boundary_width, boundary_width), char_size, line_cnt, line_text.c_str());
            // clear line_x and line_text, increment line_cnt
            line_text = word + " ";
            line_x = 0;
            line_cnt++;
            continue;
        }
    }
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
bool wtools::input(ImFont *font, char* text, std::size_t text_size,  const std::string &hint, ImGuiInputTextFlags flags, bool centered) noexcept
{
    bool used = false;

    float margin = menu::scales::margin / 2;

    ImGui::Dummy(ImVec2(0, margin));

    if (centered)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + margin);

    ImGui::PushFont(font);

    /*
     * Hint always different, label different
     */
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - margin);
    used = ImGui::InputTextWithHint(std::string("###" + hint).c_str(), hint.c_str(), text, text_size, flags);
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
    size_t text_size = text.length();

    return (text_size > 25 ? text.substr(0, static_cast<int>(text_size / 2)) + "..." : text);
}
