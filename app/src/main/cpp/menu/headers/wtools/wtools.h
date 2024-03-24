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
    void resize_text(ImFont* font, const std::string& text) noexcept;
    void center()                                           noexcept;

}

#endif //HARD_MOTION_WTOOLS_H
