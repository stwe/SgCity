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

#include "Camera.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/Window.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::camera::Camera::Camera(
    std::shared_ptr<sg::ogl::Window> t_window,
    const glm::vec3& t_position,
    const float t_yaw,
    const float t_pitch
)
    : m_window{ std::move(t_window) }
    , position{ t_position }
    , yaw{ t_yaw }
    , pitch{ t_pitch }
{
    Log::SG_LOG_DEBUG("[Camera::Camera()] Create Camera.");

    UpdateCameraVectors();
}

sg::ogl::camera::Camera::~Camera() noexcept
{
    Log::SG_LOG_DEBUG("[Camera::~Camera()] Destruct Camera.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::camera::Camera::Input()
{
    // keyboard

    if (m_window->IsKeyPressed(GLFW_KEY_W))
    {
        ProcessKeyboard(ogl::camera::Camera::Direction::FORWARD);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_S))
    {
        ProcessKeyboard(ogl::camera::Camera::Direction::BACKWARD);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_A))
    {
        ProcessKeyboard(ogl::camera::Camera::Direction::LEFT);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_D))
    {
        ProcessKeyboard(ogl::camera::Camera::Direction::RIGHT);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_O))
    {
        ProcessKeyboard(ogl::camera::Camera::Direction::UP);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_U))
    {
        ProcessKeyboard(ogl::camera::Camera::Direction::DOWN);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_I))
    {
        Log::SG_LOG_DEBUG("Camera position: x: {}, y: {}, z: {}, yaw: {}, pitch: {}",
            position.x,
            position.y,
            position.z,
            yaw,
            pitch
        );
    }

    // mouse

    if (m_window->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        auto displVec{ glm::vec2(0.0f) };
        const auto currentPosition{ m_window->GetMousePosition() };

        if (m_lastMouse.x > 0 && m_lastMouse.y > 0)
        {
            const auto deltax{ currentPosition.x - m_lastMouse.x };
            const auto deltay{ currentPosition.y - m_lastMouse.y };

            const auto rotateX{ deltax != 0 };
            const auto rotateY{ deltay != 0 };

            if (rotateX)
            {
                displVec.y = static_cast<float>(deltax);
            }

            if (rotateY)
            {
                displVec.x = static_cast<float>(deltay);
            }
        }

        m_lastMouse.x = currentPosition.x;
        m_lastMouse.y = currentPosition.y;

        ProcessMouse(displVec);
    }
}

//-------------------------------------------------
// Keyboard && Mouse
//-------------------------------------------------

void sg::ogl::camera::Camera::ProcessKeyboard(const Direction t_direction)
{
    if (t_direction == Direction::FORWARD)
        position += m_front * m_movementSpeed;
    if (t_direction == Direction::BACKWARD)
        position -= m_front * m_movementSpeed;
    if (t_direction == Direction::LEFT)
        position -= m_right * m_movementSpeed;
    if (t_direction == Direction::RIGHT)
        position += m_right * m_movementSpeed;
    if (t_direction == Direction::UP)
        position += m_up * m_movementSpeed;
    if (t_direction == Direction::DOWN)
        position -= m_up * m_movementSpeed;
}

void sg::ogl::camera::Camera::ProcessMouse(const glm::vec2& t_displVec)
{
    yaw += t_displVec.y * m_mouseSensitivity;
    pitch -= t_displVec.x * m_mouseSensitivity;

    if (yaw > 359.0f)
    {
        yaw = 359.0f;
    }
    else if (yaw < -359.0f)
    {
        yaw = -359.0f;
    }

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    else if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    UpdateCameraVectors();
}

//-------------------------------------------------
// Euler angles
//-------------------------------------------------

void sg::ogl::camera::Camera::UpdateCameraVectors()
{
    // Calculate the new Front vector.
    glm::vec3 front;
    front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    m_front = glm::normalize(front);

    // Also re-calculate the Right and Up vector.
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
