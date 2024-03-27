//
// Created by sel on 21/03/2024.
//

#include "../../global_includes/ginc.h"

#include "fonts/manrope.h"
#include "icons/icon_data.h"
#include "icons/icons.h"

#ifndef HARD_MOTION_MENU_H
#define HARD_MOTION_MENU_H

/*
 * Centralized data of the whole menu [ HARD MOTION ]
 */

namespace menu
{
    void spawn()      noexcept;
    void load_style() noexcept;

    constexpr int flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    namespace colors
    {
        inline float global_alpha = 255.f;
        inline float off_alpha    = 90.f;


        inline float global_red   = 255.f;
        inline float global_green = 255.f;
        inline float global_blue  = 255.f;

        inline ImVec4 parent    = ImVec4::FromRGBA(32,  32,  32,  global_alpha); // #202020
        inline ImVec4 child     = ImVec4::FromRGBA(39,  39,  39,  global_alpha); // #272727
        inline ImVec4 widgets   = ImVec4::FromRGBA(48,  48,  48,  global_alpha); // #303030
        inline ImVec4 scrollbar = ImVec4::FromRGBA(202, 202, 202, global_alpha); // #CACACA
        inline ImVec4 hint      = ImVec4::FromRGBA(151, 151, 151, global_alpha); // #979797
        inline ImVec4 font      = ImVec4::FromRGBA(255, 255, 255, global_alpha); // #FFFFFF
        inline ImVec4 admin     = ImVec4::FromRGBA(255, 101, 101, global_alpha); // #FF6565
    }

    namespace size
    {
        inline float scale  = 140.f;
        inline int   round  = 50;
        inline int   border = 0;
    }

    namespace font
    {
        inline ImFont* head = nullptr;
        inline ImFont* body = nullptr;
    }

    namespace widgets
    {
        void head_text(const std::string& text, bool centered = true)                                                                                       noexcept;
        void body_text(const std::string& text, bool centered = true)                                                                                       noexcept;

        bool head_button(const std::string& text, bool centered = true)                                                                                     noexcept;
        bool body_button(const std::string& text, bool centered = true)                                                                                     noexcept;

        void upper_title(const std::string& text)                                                                                                           noexcept;

        void logo()                                                                                                                                         noexcept;

        bool window_with_margins(const std::string &label, float vertical_length = 0.f, ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0) noexcept;
    }

    namespace scales
    {
        inline float option = 207.5f;

        inline float margin = 50.f;
    }
}

#endif //HARD_MOTION_MENU_H
