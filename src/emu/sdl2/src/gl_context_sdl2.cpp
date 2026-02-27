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

#include <sdl2/gl_context_sdl2.h>
#include <common/log.h>

namespace eka2l1::sdl {
    gl_context_sdl2::gl_context_sdl2(SDL_Window *window, bool is_gles)
        : window_(window)
        , gl_context_(nullptr) {
        m_opengl_mode = is_gles ? mode::opengl_es : mode::opengl;

        if (is_gles) {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        } else {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        }

        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        gl_context_ = SDL_GL_CreateContext(window_);
        if (!gl_context_) {
            LOG_ERROR(FRONTEND_CMDLINE, "SDL_GL_CreateContext failed: {}", SDL_GetError());
        }
    }

    gl_context_sdl2::~gl_context_sdl2() {
        if (gl_context_) {
            SDL_GL_DeleteContext(gl_context_);
        }
    }

    bool gl_context_sdl2::make_current() {
        return SDL_GL_MakeCurrent(window_, gl_context_) == 0;
    }

    bool gl_context_sdl2::clear_current() {
        return SDL_GL_MakeCurrent(window_, nullptr) == 0;
    }

    void gl_context_sdl2::swap_buffers() {
        SDL_GL_SwapWindow(window_);
    }

    void gl_context_sdl2::update(const std::uint32_t new_width, const std::uint32_t new_height) {
        m_backbuffer_width = new_width;
        m_backbuffer_height = new_height;
    }

    void gl_context_sdl2::set_swap_interval(const std::int32_t interval) {
        SDL_GL_SetSwapInterval(interval);
    }

    bool gl_context_sdl2::is_headless() const {
        return window_ == nullptr;
    }

    std::unique_ptr<drivers::graphics::gl_context> gl_context_sdl2::create_shared_context() {
        return nullptr;
    }
}
