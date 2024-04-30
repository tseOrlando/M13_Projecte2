//
// Created by paulo on 30/04/2024.
//

#ifndef HARD_MOTION_ANIMATION_H
#define HARD_MOTION_ANIMATION_H

#include "engine.h"
#include "main/cpp/menu/headers/menu.h"

class dot
{
public:
    dot(source_engine::vector p, source_engine::vector v)
    {
        m_vel = v;
        m_pos = p;
    }

    inline void update();
    inline void draw();

    source_engine::vector m_pos, m_vel;
};

static std::vector<dot*> dots = { };

inline void dot::update()
{
    float opacity = 255.0f / 255.0f;

    m_pos += m_vel * (opacity);
}

inline void dot::draw()
{
    ImGui::GetWindowDrawList()->AddCircleFilled({ m_pos.x - 2.f, m_pos.y - 2.f }, 1, ImColor(menu::colors::global_red, menu::colors::global_green, menu::colors::global_blue, menu::colors::off_alpha));

    auto t = std::find(dots.begin(), dots.end(), this);

    if (t == dots.end())
        return;

    for (auto i = t; i != dots.end(); i++)
    {
        if ((*i) == this) continue;

        int dist = (m_pos - (*i)->m_pos).length_2d();

        if (dist < 170) //soon customizable
            ImGui::GetWindowDrawList()->AddLine({ m_pos.x - 2, m_pos.y - 3 }, { (*i)->m_pos.x - 2, (*i)->m_pos.y - 3 }, ImColor(menu::colors::global_red, menu::colors::global_green, menu::colors::global_blue, menu::colors::off_alpha), 1);
    }
}

inline void dot_draw()
{
    struct screen_size
    {
        int x, y;
    }; screen_size sc;

    RECT screen_rect;
    GetWindowRect(GetDesktopWindow(), &screen_rect);

    sc.x = GetSystemMetrics(SM_CXSCREEN / 2);
    sc.y = GetSystemMetrics(SM_CYSCREEN / 2);

    using namespace random_;

    int s = random_num(0, 14);

    if (s == 0)
    {
        dots.push_back(new dot(source_engine::vector(random_num<int>(0, sc.x), -16, 0), source_engine::vector(random_num<int>(-3, 3), random_num<int>(1, 3), 0)));
    }
    else if (s == 1)
    {
        dots.push_back(new dot(source_engine::vector(random_num<int>(0, sc.x), random_num<int>(sc.y + 16), 0), source_engine::vector(random_num<int>(-3, 3), -1 * random_num<int>(1, 3), 0)));
    }
    else if (s == 2)
    {
        dots.push_back(new dot(source_engine::vector(-16, random_num<int>(0, sc.y), 0), source_engine::vector(random_num<int>(1, 3), random_num<int>(-3, 3), 0)));
    }
    else if (s == 3)
    {
        dots.push_back(new dot(source_engine::vector(random_num<int>(sc.x + 16), random_num<int>(0, sc.y), 0), source_engine::vector(-1 * random_num<int>(1, 3), random_num<int>(-3, 3), 0)));
    }

    auto alph = 255.0f * (255.0f / 255.0f);
    auto a_int = static_cast<int>(alph);

    ImGui::GetWindowDrawList()->AddRectFilled({ 0, 0 }, { static_cast<float>(sc.x), static_cast<float>(sc.y) }, ImColor(a_int, 0, 0, 0));

    for (auto i = dots.begin(); i < dots.end();)
    {
        if ((*i)->m_pos.y <= -20 || (*i)->m_pos.y > sc.y + 20 || (*i)->m_pos.x < -20 || (*i)->m_pos.x > sc.x + 20)
        {
            delete (*i);
            i = dots.erase(i);
        }
        else
        {
            (*i)->update();
            i++;
        }
    }

    for (auto i = dots.begin(); i < dots.end(); i++)
        (*i)->draw();
}

inline void dot_destroy()
{
    for (auto i = dots.begin(); i < dots.end(); i++)
        delete (*i);

    dots.clear();
}


#endif //HARD_MOTION_ANIMATION_H
