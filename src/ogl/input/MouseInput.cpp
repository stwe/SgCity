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

#include "MouseInput.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Singleton
//-------------------------------------------------

sg::ogl::input::MouseInput& sg::ogl::input::MouseInput::GetInstance()
{
    static MouseInput instance;
    return instance;
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::input::MouseInput::Init(const Window& t_window)
{
    // Registers a callback that will be invoked when the mouse is moved.
    glfwSetCursorPosCallback
        (
            t_window.GetWindowHandle(),
            [](GLFWwindow* t_window, const double t_x, const double t_y)
            {
                GetInstance().m_currentPosition.x = t_x;
                GetInstance().m_currentPosition.y = t_y;
            }
        );

    // Registers a callback that will be invoked when the mouse enters our window.
    glfwSetCursorEnterCallback
        (
            t_window.GetWindowHandle(),
            [](GLFWwindow* t_window, const int t_entered)
            {
                GetInstance().m_inWindow = t_entered;
            }
        );

    // Registers a callback that will be invoked when a mouse button is pressed.
    glfwSetMouseButtonCallback
        (
            t_window.GetWindowHandle(),
            [](GLFWwindow* t_window, const int t_button, const int t_action, const int t_mods)
            {
                GetInstance().m_leftButtonPressed = t_button == GLFW_MOUSE_BUTTON_1 && t_action == GLFW_PRESS;
                GetInstance().m_rightButtonPressed = t_button == GLFW_MOUSE_BUTTON_2 && t_action == GLFW_PRESS;
            }
        );
}

void sg::ogl::input::MouseInput::Input()
{
    m_displVec.x = 0.0f;
    m_displVec.y = 0.0f;

    if (m_previousPosition.x > 0 && m_previousPosition.y > 0 && m_inWindow)
    {
        const auto deltax{ m_currentPosition.x - m_previousPosition.x };
        const auto deltay{ m_currentPosition.y - m_previousPosition.y };

        const auto rotateX = deltax != 0;
        const auto rotateY = deltay != 0;

        if (rotateX)
        {
            m_displVec.y = static_cast<float>(deltax);
        }

        if (rotateY)
        {
            m_displVec.x = static_cast<float>(deltay);
        }
    }

    m_previousPosition.x = m_currentPosition.x;
    m_previousPosition.y = m_currentPosition.y;
}
