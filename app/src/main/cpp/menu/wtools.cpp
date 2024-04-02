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
void wtools::text(ImFont* font, const std::string& text, bool centered) noexcept
{
    if (centered)
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text.c_str()).x) * 0.5f);

    ImGui::PushFont(font);
    ImGui::Text("%s", text.c_str());
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

bool wtools::input(ImFont *font, char* text, const std::string &hint, ImGuiInputTextFlags flags, bool centered) noexcept
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
    used = ImGui::InputText(std::string("###" + hint).c_str(), text, sizeof text, flags);

    ImGui::PopFont();

    ImGui::Dummy(ImVec2(0, margin / 4));

    return used;
}
