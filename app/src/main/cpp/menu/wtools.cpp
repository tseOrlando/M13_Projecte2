//
// Created by sel on 24/03/2024.
//

#include "main/cpp/menu/headers/menu.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

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
 * Changes the button font, in result of 'resizing' it, in my app context
 * it represents that,  only used for 'head_button' and 'body_button'
 */
bool wtools::resize_button(ImFont *font, const std::string &text) noexcept
{
    bool used = false;

    ImGui::PushFont(font);

    used = ImGui::Button(text.c_str());

    ImGui::PopFont();

    return used;
}

/*
 * Centers whatever is called ( as long as is a ImGui widget ) below of the call of 'center'
 * The y parameter makes it available to align to a certain y-axis offset
 * The x parameter depends on the widget size meaning you still have to calculate the total
 * x value in order to the item be correctly aligned
 */
void wtools::center(float x, float y) noexcept { ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - x) * 0.5f, y)); }

/*
 * Loads an image from memory with OpenGL.
 * First loading it from the memory and uploading it to the GPU
 * binding and using ImGui API to load it into the UI
 */
void wtools::load_image(const unsigned char* data, size_t data_size, ImVec2 size) noexcept
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

        return ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture)), size);;
    }

    return;
}
