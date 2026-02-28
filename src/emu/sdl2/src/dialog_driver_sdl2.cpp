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

#include <drivers/ui/input_dialog.h>
#include <common/applauncher.h>
#include <common/log.h>

#include <cstdlib>

namespace eka2l1::common {
    bool launch_browser(const std::string &url) {
        std::string cmd = "xdg-open '" + url + "' &";
        return std::system(cmd.c_str()) == 0;
    }
}

namespace eka2l1::drivers::ui {
    bool open_input_view(const std::u16string &initial_text, const int max_len,
        input_dialog_complete_callback complete_callback) {
        LOG_WARN(FRONTEND_CMDLINE, "Input dialog not implemented in SDL2 frontend");
        return false;
    }

    void close_input_view() {
    }

    void show_yes_no_dialog(const std::u16string &text, const std::u16string &button1_text,
        const std::u16string &button2_text, yes_no_dialog_complete_callback complete_callback) {
        LOG_WARN(FRONTEND_CMDLINE, "Yes/No dialog not implemented in SDL2 frontend");
        if (complete_callback) {
            complete_callback(0);
        }
    }
}
