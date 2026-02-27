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

#include <sdl2/emu_window_sdl2.h>
#include <common/log.h>

#include <SDL2/SDL_syswm.h>

namespace eka2l1::sdl {
    emu_window_sdl2::emu_window_sdl2()
        : sdl_window_(nullptr)
        , userdata_(nullptr)
        , should_quit_(false)
        , cursor_visible_(true) {
    }

    emu_window_sdl2::~emu_window_sdl2() {
        shutdown();
    }

    void emu_window_sdl2::init(std::string title, vec2 size, const std::uint32_t flags) {
        Uint32 sdl_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

        if (flags & drivers::emu_window_flag_fullscreen)
            sdl_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

        sdl_window_ = SDL_CreateWindow(title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            size.x, size.y, sdl_flags);

        if (!sdl_window_) {
            LOG_ERROR(FRONTEND_CMDLINE, "SDL_CreateWindow failed: {}", SDL_GetError());
        }
    }

    void emu_window_sdl2::poll_events() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                should_quit_ = true;
                if (close_hook)
                    close_hook(userdata_);
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED ||
                    event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    if (resize_hook) {
                        int w, h;
                        SDL_GL_GetDrawableSize(sdl_window_, &w, &h);
                        resize_hook(userdata_, vec2(w, h));
                    }
                }
                break;

            case SDL_KEYDOWN:
                if (!event.key.repeat && button_pressed) {
                    button_pressed(userdata_, static_cast<std::uint32_t>(event.key.keysym.sym));
                }
                break;

            case SDL_KEYUP:
                if (!event.key.repeat && button_released) {
                    button_released(userdata_, static_cast<std::uint32_t>(event.key.keysym.sym));
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (raw_mouse_event) {
                    int button = 0;
                    if (event.button.button == SDL_BUTTON_RIGHT) button = 1;
                    else if (event.button.button == SDL_BUTTON_MIDDLE) button = 2;
                    raw_mouse_event(userdata_, vec3(event.button.x, event.button.y, 0), button, 0, 0);
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (raw_mouse_event) {
                    int button = 0;
                    if (event.button.button == SDL_BUTTON_RIGHT) button = 1;
                    else if (event.button.button == SDL_BUTTON_MIDDLE) button = 2;
                    raw_mouse_event(userdata_, vec3(event.button.x, event.button.y, 0), button, 2, 0);
                }
                break;

            case SDL_MOUSEMOTION:
                if (raw_mouse_event) {
                    int button = -1;
                    if (event.motion.state & SDL_BUTTON_LMASK) button = 0;
                    else if (event.motion.state & SDL_BUTTON_RMASK) button = 1;
                    raw_mouse_event(userdata_, vec3(event.motion.x, event.motion.y, 0), button, 1, 0);
                }
                break;

            default:
                break;
            }
        }
    }

    void emu_window_sdl2::shutdown() {
        if (sdl_window_) {
            SDL_DestroyWindow(sdl_window_);
            sdl_window_ = nullptr;
        }
    }

    void emu_window_sdl2::set_fullscreen(const bool is_fullscreen) {
        if (sdl_window_) {
            SDL_SetWindowFullscreen(sdl_window_, is_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
        }
    }

    bool emu_window_sdl2::should_quit() {
        return should_quit_;
    }

    void emu_window_sdl2::change_title(std::string title) {
        if (sdl_window_) {
            SDL_SetWindowTitle(sdl_window_, title.c_str());
        }
    }

    void emu_window_sdl2::swap_buffer() {
        // The gl_context (EGL/GLX) handles the actual buffer swap via
        // eglSwapBuffers/glXSwapBuffers. Nothing to do here.
    }

    vec2 emu_window_sdl2::window_size() {
        int w = 0, h = 0;
        if (sdl_window_)
            SDL_GetWindowSize(sdl_window_, &w, &h);
        return vec2(w, h);
    }

    vec2 emu_window_sdl2::window_fb_size() {
        int w = 0, h = 0;
        if (sdl_window_)
            SDL_GL_GetDrawableSize(sdl_window_, &w, &h);
        return vec2(w, h);
    }

    vec2d emu_window_sdl2::get_mouse_pos() {
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);
        return vec2d({ static_cast<double>(x), static_cast<double>(y) });
    }

    bool emu_window_sdl2::get_mouse_button_hold(const int mouse_btt) {
        Uint32 state = SDL_GetMouseState(nullptr, nullptr);
        switch (mouse_btt) {
        case 0: return (state & SDL_BUTTON_LMASK) != 0;
        case 1: return (state & SDL_BUTTON_RMASK) != 0;
        case 2: return (state & SDL_BUTTON_MMASK) != 0;
        default: return false;
        }
    }

    void emu_window_sdl2::set_userdata(void *userdata) {
        userdata_ = userdata;
    }

    void *emu_window_sdl2::get_userdata() {
        return userdata_;
    }

    bool emu_window_sdl2::set_cursor(drivers::cursor *cur) {
        return true;
    }

    void emu_window_sdl2::cursor_visiblity(const bool visi) {
        cursor_visible_ = visi;
        SDL_ShowCursor(visi ? SDL_ENABLE : SDL_DISABLE);
    }

    bool emu_window_sdl2::cursor_visiblity() {
        return cursor_visible_;
    }

    drivers::window_system_info emu_window_sdl2::get_window_system_info() {
        drivers::window_system_info wsi;

        if (!sdl_window_) {
            wsi.type = drivers::window_system_type::headless;
            return wsi;
        }

        SDL_SysWMinfo wminfo;
        SDL_VERSION(&wminfo.version);

        if (!SDL_GetWindowWMInfo(sdl_window_, &wminfo)) {
            LOG_ERROR(FRONTEND_CMDLINE, "SDL_GetWindowWMInfo failed: {}", SDL_GetError());
            wsi.type = drivers::window_system_type::headless;
            return wsi;
        }

        switch (wminfo.subsystem) {
#if defined(SDL_VIDEO_DRIVER_X11)
        case SDL_SYSWM_X11:
            wsi.type = drivers::window_system_type::x11;
            wsi.display_connection = wminfo.info.x11.display;
            wsi.render_window = reinterpret_cast<void *>(wminfo.info.x11.window);
            wsi.render_surface = wsi.render_window;
            break;
#endif
#if defined(SDL_VIDEO_DRIVER_WAYLAND)
        case SDL_SYSWM_WAYLAND:
            wsi.type = drivers::window_system_type::wayland;
            wsi.display_connection = wminfo.info.wl.display;
            wsi.render_window = wminfo.info.wl.surface;
            wsi.render_surface = wsi.render_window;
            break;
#endif
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
        case SDL_SYSWM_WINDOWS:
            wsi.type = drivers::window_system_type::windows;
            wsi.render_window = reinterpret_cast<void *>(wminfo.info.win.window);
            wsi.render_surface = wsi.render_window;
            break;
#endif
#if defined(SDL_VIDEO_DRIVER_COCOA)
        case SDL_SYSWM_COCOA:
            wsi.type = drivers::window_system_type::macOS;
            wsi.render_window = reinterpret_cast<void *>(wminfo.info.cocoa.window);
            wsi.render_surface = wsi.render_window;
            break;
#endif
        default:
            wsi.type = drivers::window_system_type::headless;
            break;
        }

        int w = 0, h = 0;
        SDL_GL_GetDrawableSize(sdl_window_, &w, &h);
        wsi.surface_width = static_cast<std::uint32_t>(w);
        wsi.surface_height = static_cast<std::uint32_t>(h);
        wsi.render_surface_scale = 1.0f;

        int window_w = 0;
        SDL_GetWindowSize(sdl_window_, &window_w, nullptr);
        if (window_w > 0)
            wsi.render_surface_scale = static_cast<float>(w) / static_cast<float>(window_w);

        return wsi;
    }
}
