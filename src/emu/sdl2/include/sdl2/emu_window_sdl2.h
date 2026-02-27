/*
 * Copyright (c) 2024 EKA2L1 Team.
 *
 * This file is part of EKA2L1 project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <drivers/graphics/emu_window.h>
#include <SDL2/SDL.h>

namespace eka2l1::sdl {
    class emu_window_sdl2 : public drivers::emu_window {
    public:
        emu_window_sdl2();
        ~emu_window_sdl2() override;

        void init(std::string title, vec2 size, const std::uint32_t flags) override;
        void poll_events() override;
        void shutdown() override;
        void set_fullscreen(const bool is_fullscreen) override;
        bool should_quit() override;
        void change_title(std::string title) override;

        void swap_buffer() override;

        vec2 window_size() override;
        vec2 window_fb_size() override;
        vec2d get_mouse_pos() override;

        bool get_mouse_button_hold(const int mouse_btt) override;
        void set_userdata(void *userdata) override;
        void *get_userdata() override;

        bool set_cursor(drivers::cursor *cur) override;
        void cursor_visiblity(const bool visi) override;
        bool cursor_visiblity() override;

        drivers::window_system_info get_window_system_info() override;

        SDL_Window *get_sdl_window() { return sdl_window_; }

    private:
        SDL_Window *sdl_window_;
        void *userdata_;
        bool should_quit_;
        bool cursor_visible_;
    };
}
