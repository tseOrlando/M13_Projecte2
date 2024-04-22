//
// Created by sel on 21/03/2024.
//

#include "../../global_includes/ginc.h"

#include "fonts/manrope.h"
#include "icons/icon_data.h"
#include "icons/icons.h"
#include "main/cpp/menu/entities/event_t.h"

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

    namespace values
    {
        inline std::vector<const char *> dances = {"jumpstyle", "gabber/hakken", "shuffle" };

        inline std::vector<event_t> events;

        inline ImVec2 get_font_size(ImFont* font) noexcept;

        namespace user_data
        {
            inline char user_name[16] = {};
            inline char pass_word[16] = {};
            inline char e_mail[32]    = {};
            inline char number[9]     = {};
            inline const char* dance;

            inline bool logged_in = false;
        }
    }

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

    namespace font
    {
        inline ImFont* head = nullptr;
        inline ImFont* body = nullptr;
        inline ImFont* foot = nullptr;
    }

    namespace widgets
    {
        void head_text(const std::string& text, bool multi_line = false, bool disabled = false, bool centered = true)                                         noexcept;
        void body_text(const std::string& text, bool multi_line = false, bool disabled = false, bool centered = true)                                         noexcept;
        void foot_text(const std::string& text, bool multi_line = false, bool disabled = false, bool centered = true)                                         noexcept;

        bool head_button(const std::string& text, bool centered = true)                                                              noexcept;
        bool body_button(const std::string& text, bool centered = true)                                                              noexcept;
        bool foot_button(const std::string& text, bool centered = true)                                                              noexcept;
        bool input(char* text, std::size_t text_size, const std::string& hint, bool multi_line = false, const std::string& icon = "", ImGuiInputTextFlags iflags = 0, bool centered = true) noexcept;
        bool input_foot(char* text, std::size_t text_size, const std::string& hint, bool multi_line = false, const std::string& icon = "", ImGuiInputTextFlags iflags = 0, bool centered = true) noexcept;
        const char* combo(const char *label, const char* preview_value, std::vector<const char*> items, bool centered = true)        noexcept;
        void upper_title(const std::string& text)                                                                                    noexcept;

        void logo()                                                                                                                                                                      noexcept;

        bool window_with_margins(const std::string &label, float vertical_length = 0.f, float vertical_margin = 0.f, ImVec4 col = colors::child, ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0) noexcept;
        void end_window_with_margins(float vertical_margin = 0.f)                                                                                                                        noexcept;

        bool event(const std::string &label, const std::string& info, std::function<void(const std::string& title, const std::string& content)> post_call_func = nullptr, float vertical_length = 0.f, float vertical_margin = 0.f) noexcept;
        void bulk(float more = 0)                                                                                                                                                                                                                 noexcept;
    }

    namespace scales
    {
        inline float option = 218.f;
        inline float input  = 190.f;
        inline float combo  = 160.f;
        inline float scale  = 140.f;
        inline float event  = 370.f;
        inline int   round  = 50;
        inline int   border = 0;

        inline float margin = 50.f;
        inline float slight_space_between_widgets = margin / 5;
        inline float event_margin = margin / 3;
    }

    namespace core
    {
        enum tab_t : int
        {
            _landing        = 0,
            _log_in         = 1,
            _register_in    = 2,
            _hub            = 3,
            _events         = 4,
            _event_info     = 5,
            _event_members  = 6,
            _member_info    = 7,
            _create_event   = 8,
            _joined_events  = 9,
            _search         = 10,
            _filter         = 11,
            _user           = 12,
            _edit_user_info = 13,
            _admin_panel    = 14
        };

        void go_to_tab(tab_t tab) noexcept;
        void go_back()            noexcept;

        inline bool change_tab = false;

        inline tab_t current_tab = tab_t::_landing;

        namespace lobby
        {
            void                   landing() noexcept;

            namespace auth
            {
                void                log_in() noexcept;
                void           register_in() noexcept;
            }

            namespace main
            {
                void                   hub() noexcept;

                namespace events
                {
                    void            events() noexcept;
                    void        event_info() noexcept;

                    namespace members
                    {
                        void event_members() noexcept;
                        void   member_info() noexcept;
                    }

                    void      create_event() noexcept;
                    void     joined_events() noexcept;
                }

                namespace search
                {
                    void            search() noexcept;
                    void            filter() noexcept;
                }

                namespace user
                {
                    void              user() noexcept;
                    void    edit_user_info() noexcept;
                    void       admin_panel() noexcept;
                }
            }
        }
    }
}

#endif //HARD_MOTION_MENU_H