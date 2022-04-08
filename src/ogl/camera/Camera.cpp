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
#include "event/EventManager.h"
#include "eventpp/utilities/argumentadapter.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::camera::Camera::Camera(
    std::shared_ptr<Window> t_window,
    const glm::vec3& t_position,
    const float t_yaw,
    const float t_pitch
)
    : position{ t_position }
    , yaw{ t_yaw }
    , pitch{ t_pitch }
    , m_window{ std::move(t_window) }
{
    Log::SG_LOG_DEBUG("[Camera::Camera()] Create Camera.");

    InitListeners();
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
        ProcessKeyboard(Direction::FORWARD);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_S))
    {
        ProcessKeyboard(Direction::BACKWARD);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_A))
    {
        ProcessKeyboard(Direction::LEFT);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_D))
    {
        ProcessKeyboard(Direction::RIGHT);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_O))
    {
        ProcessKeyboard(Direction::UP);
    }

    if (m_window->IsKeyPressed(GLFW_KEY_U))
    {
        ProcessKeyboard(Direction::DOWN);
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

    m_displVec.x = 0.0f;
    m_displVec.y = 0.0f;

    m_currentPosition = m_window->GetMousePosition();

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

    if (m_window->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        ProcessMouse();
    }
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::camera::Camera::InitListeners()
{
    Log::SG_LOG_DEBUG("[Camera::InitListeners()] Append listeners.");

    event::EventManager::eventDispatcher.appendListener(
        event::SgEventType::MOUSE_ENTER,
        eventpp::argumentAdapter<void(const event::MouseEnterEvent&)>(
            [this](const event::MouseEnterEvent& t_event)
            {
                m_inWindow = t_event.enter;
            }
        )
    );
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

void sg::ogl::camera::Camera::ProcessMouse()
{
    yaw += m_displVec.y * m_mouseSensitivity;
    pitch -= m_displVec.x * m_mouseSensitivity;

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
