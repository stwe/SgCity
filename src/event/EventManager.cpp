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

#include "EventManager.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::event::EventManager::SetKeyboardGlfwCallbacks(const ogl::Window& t_window)
{
    // Registers a callback that will be invoked when a key is pressed or released
    glfwSetKeyCallback
    (
        t_window.GetWindowHandle(),
        [](GLFWwindow* t_window, const int t_key, const int t_scanCode, const int t_action, const int t_mods)
        {
            switch (t_action)
            {
            case GLFW_PRESS:
                eventDispatcher.dispatch(SgEventType::KEY_PRESSED, KeyPressedEvent(t_key));
                break;
            case GLFW_RELEASE:
                eventDispatcher.dispatch(SgEventType::KEY_RELEASED, KeyReleasedEvent(t_key));
                break;
            case GLFW_REPEAT:
                eventDispatcher.dispatch(SgEventType::KEY_PRESSED, KeyPressedEvent(t_key, 1));
                break;
            default:;
            }
        }
    );
}

void sg::event::EventManager::SetMouseGlfwCallbacks(const ogl::Window& t_window)
{
    // Registers a callback that will be invoked when the mouse is moved.
    glfwSetCursorPosCallback
    (
        t_window.GetWindowHandle(),
        [](GLFWwindow* t_window, const double t_x, const double t_y)
        {
            eventDispatcher.dispatch(
                SgEventType::MOUSE_MOVED,
                MouseMovedEvent(
                    static_cast<float>(t_x),
                    static_cast<float>(t_y)
                    )
            );
        }
    );

    // Registers a callback that will be invoked when a scrolling device is used.
    glfwSetScrollCallback
    (
        t_window.GetWindowHandle(),
        [](GLFWwindow* t_window, const double t_xOffset, const double t_yOffset)
        {
            eventDispatcher.dispatch(
                SgEventType::MOUSE_SCROLLED,
                MouseScrolledEvent(
                    static_cast<float>(t_xOffset),
                    static_cast<float>(t_yOffset)
                    )
            );
        }
    );

    // Registers a callback that will be invoked when the mouse enters our window.
    glfwSetCursorEnterCallback
    (
        t_window.GetWindowHandle(),
        [](GLFWwindow* t_window, const int t_entered)
        {
            eventDispatcher.dispatch(SgEventType::MOUSE_ENTER, MouseEnterEvent(t_entered));
        }
    );

    // Registers a callback that will be invoked when a mouse button is pressed or released.
    glfwSetMouseButtonCallback
    (
        t_window.GetWindowHandle(),
        [](GLFWwindow* t_window, const int t_button, const int t_action, const int t_mods)
        {
            switch (t_action)
            {
            case GLFW_PRESS:
                eventDispatcher.dispatch(SgEventType::MOUSE_BUTTON_PRESSED, MouseButtonPressedEvent(t_button));
                break;
            case GLFW_RELEASE:
                eventDispatcher.dispatch(SgEventType::MOUSE_BUTTON_RELEASED, MouseButtonReleasedEvent(t_button));
                break;
            default:;
            }
        }
    );
}
