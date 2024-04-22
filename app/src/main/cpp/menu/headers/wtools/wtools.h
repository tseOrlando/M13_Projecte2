//
// Created by sel on 24/03/2024.
//

#ifndef HARD_MOTION_WTOOLS_H
#define HARD_MOTION_WTOOLS_H

/*
 * This namespace contains functions to help on the development
 * of Hard Motion menu exclusively, it's adapted for it's own
 * context, it may work generally but the result it's unexpected
 * in other sources.
 *
 * The functions are declared on 'wtools.cpp'
 */
namespace wtools
{
    void text(ImFont* font, const std::string& text, bool multi_line, bool disabled, bool centered)                                                  noexcept;
    void wrapped_text(ImFont* font, std::string text, ImVec2 position, int boundary_width, int lines = 3, int y_padding = 3)        noexcept;
    bool button(ImFont* font, const std::string& text, bool centered)                                                               noexcept;
    bool input(ImFont* font, char* text, std::size_t text_size,  const std::string& hint, bool multi_line, const std::string& icon, ImGuiInputTextFlags flags, bool centered) noexcept;
    const char* combo(ImFont* font, const char *label, const char* preview_value, std::vector<const char*> items, bool centered)    noexcept;
    void image(const unsigned char* data, size_t data_size, ImVec2 size, bool centered)                                             noexcept;

    std::string get_curiosity_text(const std::string& text)                                                                         noexcept;
    void align()                                                                                                                    noexcept;
}

/* OLD
 * This code is taken from 'https://github.com/ocornut/imgui/discussions/3862' and it's not mine.
 * Since ImGui is a bit hard to accommodate sometimes, I need a good centerer to make the UI faster
 * PD : Slight modified by me to adapt to my own context
 */
class centered_control_t
{
    class centered_control_wrapper_t
    {

    public:

        explicit centered_control_wrapper_t(bool result) : result_(result) {}

        operator bool() const { return result_; }

    private:

        bool result_;

    };

public:

    centered_control_t(ImVec2 window_size, float y = 0.f, float s = 0.f, float su = 0.f) : window_size_(window_size) { y_offset = y; spacing_below = s; spacing_up = su;}

    /*
     * TODO : "Find a way to make it center without adding 2 same widgets"
     */
    template<typename func>
    centered_control_wrapper_t operator()(func control)
    {
        centered_control_wrapper_t ccw = centered_control_wrapper_t(false);

        original_pos = ImGui::GetCursorPos();

        ImGui::SetCursorPos(change ? ImVec2((window_size_.x - control_size_x) * 0.5f, original_pos.y + y_offset) : original_pos);
        control();

        control_size_x = ImGui::GetItemRectSize().x;
        change = true;

        ImGui::Dummy(ImVec2(0, spacing_below));

        return ccw;
    }

private:

    bool change = false;

    ImVec2 window_size_;

    ImVec2 original_pos;
    float control_size_x;

    float y_offset;
    float spacing_below;
    float spacing_up;
};

/*
 * This macros centers in the better way possible, got other function but this one may work
 * for the 90% of widgets of ImGui so it's ok, because for some reason with 'ImGui::BeginChild'
 * doesn't work properly
 *
 * In the __VA_ARGS__ which is pointed to the '...' as the second parameter, means that you can
 * call the macro with more arguments. It may not look safe but i'll explain briefly below
 *
 * First arg  : function ( No matter the type )
 * Second arg : vertical offset to center on
 * Third arg  : add spacing below the widget
 * Fourth arg : add spacing over the widget
 *
 * Cons : to add only spacing you'll need to still add a y_offset, you can do it with 0.f on the
 * second one, to avoid redundancy I made it like that
 */
//#define center_(control, ...) centered_control_t{ImGui::GetWindowSize(), __VA_ARGS__}([&]() { control; })

#endif //HARD_MOTION_WTOOLS_H
