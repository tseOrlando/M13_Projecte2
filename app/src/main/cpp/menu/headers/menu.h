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
 * Centralized data of the whole menu
 */

namespace menu
{
    void spawn() noexcept;
    void load_style() noexcept;

    constexpr int flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    namespace size
    {
        inline float scale = 90.f;
    }

    namespace font
    {
        inline ImFont* head = nullptr;
        inline ImFont* body = nullptr;
    }
}

#endif //HARD_MOTION_MENU_H
