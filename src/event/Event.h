// This file is part of the SgCity project.
//
// Copyright (c) 2022. stwe <https://github.com/stwe/SgCity>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

namespace sg::event
{
    //-------------------------------------------------
    // Event types
    //-------------------------------------------------

    enum class SgEventType
    {
        // default
        NONE,

        // keyboard
        KEY_PRESSED, KEY_RELEASED,

        // mouse
        MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_MOVED, MOUSE_SCROLLED
    };

    //-------------------------------------------------
    // Event base class
    //-------------------------------------------------

    struct SgEvent
    {
        virtual ~SgEvent() = default;

        SgEventType type{ SgEventType::NONE };
    };

    //-------------------------------------------------
    // Keyboard
    //-------------------------------------------------

    struct KeyPressedEvent : SgEvent
    {
        explicit KeyPressedEvent(const int t_key, const int t_repeatCount = 0)
            : key{ t_key }
            , repeatCount{ t_repeatCount }
        {
            type = SgEventType::KEY_PRESSED;
        }

        int key;
        int repeatCount;
    };

    struct KeyReleasedEvent : SgEvent
    {
        explicit KeyReleasedEvent(const int t_key)
            : key{ t_key }
        {
            type = SgEventType::KEY_RELEASED;
        }

        int key;
    };

    //-------------------------------------------------
    // Mouse
    //-------------------------------------------------

    struct MouseButtonPressedEvent : SgEvent
    {
        int button;

        explicit MouseButtonPressedEvent(const int t_button)
            : button{ t_button }
        {
            type = SgEventType::MOUSE_BUTTON_PRESSED;
        }
    };

    struct MouseButtonReleasedEvent : SgEvent
    {
        int button;

        explicit MouseButtonReleasedEvent(const int t_button)
            : button{ t_button }
        {
            type = SgEventType::MOUSE_BUTTON_RELEASED;
        }
    };

    struct MouseMovedEvent : SgEvent
    {
        float x;
        float y;

        MouseMovedEvent(const float t_x, const float t_y)
            : x{ t_x }
            , y{ t_y }
        {
            type = SgEventType::MOUSE_MOVED;
        }
    };

    struct MouseScrolledEvent : SgEvent
    {
        float xOffset;
        float yOffset;

        MouseScrolledEvent(const float t_xOffset, const float t_yOffset)
            : xOffset(t_xOffset),
              yOffset(t_yOffset)
        {
            type = SgEventType::MOUSE_SCROLLED;
        }
    };
}
