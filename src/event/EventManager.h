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

#include "Event.h"
#include "eventpp/eventdispatcher.h"
#include "ogl/OpenGL.h"

namespace sg::event
{
    class EventManager
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        inline static eventpp::EventDispatcher<SgEventType, void(const SgEvent&)> eventDispatcher;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        EventManager(const EventManager& t_other) = delete;
        EventManager(EventManager&& t_other) noexcept = delete;
        EventManager& operator=(const EventManager& t_other) = delete;
        EventManager& operator=(EventManager&& t_other) noexcept = delete;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        static void SetKeyboardGlfwCallbacks(GLFWwindow* m_windowHandle);
        static void SetMouseGlfwCallbacks(GLFWwindow* m_windowHandle);

    protected:

    private:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        EventManager() = default;
        ~EventManager() noexcept = default;
    };
}
