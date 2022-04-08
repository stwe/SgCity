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

#include <memory>
#include <glm/gtc/matrix_transform.hpp>

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl
{
    class Window;
}

//-------------------------------------------------
// Camera
//-------------------------------------------------

namespace sg::ogl::camera
{
    /**
     * A camera class that processes input and calculates
     * the corresponding Euler Angles, Vectors and Matrices
     * for use in OpenGL
     */
    class Camera
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * The camera velocity.
         */
        static constexpr auto MOVEMENT_SPEED{ 1.0f / 60.0f / 4.0f };

        /**
         * Mouse movement multiplicator.
         */
        static constexpr auto MOUSE_SENSITIVITY{ 0.2f };

        /**
         * The default yaw.
         */
        static constexpr auto DEFAULT_YAW{ 90.0f };

        /**
         * The default pitch.
         */
        static constexpr auto DEFAULT_PITCH{ 0.0f };

        //-------------------------------------------------
        // Type
        //-------------------------------------------------

        /**
         * Defines several possible options for camera movement.
         */
        enum class Direction
        {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT,
            UP,
            DOWN
        };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The camera position.
         */
        glm::vec3 position{ glm::vec3(0.0f) };

        /**
         * Represents the magnitude we're looking to the left or to the right.
         */
        float yaw{ DEFAULT_YAW };

        /**
         * The angle that depicts how much we're looking up or down.
         */
        float pitch{ DEFAULT_PITCH };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Camera() = delete;

        /**
         * Constructs a new Camera object.
         *
         * @param t_window The Window object.
         * @param t_position The camera position.
         * @param t_yaw Rotation around the vertical axis.
         * @param t_pitch Rotation around the side-to-side.
         */
        Camera(
            std::shared_ptr<Window> t_window,
            const glm::vec3& t_position,
            float t_yaw = DEFAULT_YAW,
            float t_pitch = DEFAULT_PITCH
        );

        Camera(const Camera& t_other) = delete;
        Camera(Camera&& t_other) noexcept = delete;
        Camera& operator=(const Camera& t_other) = delete;
        Camera& operator=(Camera&& t_other) noexcept = delete;

        ~Camera() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        /**
         * Camera specific input logic.
         */
        void Input();

        //-------------------------------------------------
        // View matrix
        //-------------------------------------------------

        /**
         * Returns the view matrix calculated using Euler Angles and the LookAt Matrix.
         */
        [[nodiscard]] auto GetViewMatrix() const { return glm::lookAt(position, position + m_front, m_up); }

        //-------------------------------------------------
        // Setter
        //-------------------------------------------------

        /**
         * Set camera velocity.
         *
         * @param t_speed The speed value.
         */
        void SetCameraSpeed(const float t_speed) { m_movementSpeed = t_speed; }

        /**
         * Set mouse sensitivity.
         *
         * @param t_sensitivity The sensitivity value.
         */
        void SetMouseSensitivity(const float t_sensitivity) { m_mouseSensitivity = t_sensitivity; }

        /**
         * Invert pitch angle.
         */
        void InvertPitch()
        {
            pitch = -pitch;
            UpdateCameraVectors();
        }

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * A Window object.
         */
        std::shared_ptr<Window> m_window;

        /**
         * The camera velocity.
         */
        float m_movementSpeed{ MOVEMENT_SPEED };

        /**
         * The front vector.
         */
        glm::vec3 m_front{ glm::vec3(0.0f, 0.0f, 1.0f) };

        /**
         * The up axis.
         */
        glm::vec3 m_up{ glm::vec3(0.0f, 1.0f, 0.0f) };

        /**
         * The world up vector.
         */
        glm::vec3 m_worldUp{ glm::vec3(0.0f, 1.0f, 0.0f) };

        /**
         * The right axis.
         */
        glm::vec3 m_right{ glm::vec3(1.0f, 0.0f, 0.0f) };

        /**
         * The mouse sensitivity.
         */
        float m_mouseSensitivity{ MOUSE_SENSITIVITY };

        /**
         * The previous mouse position.
         */
        glm::vec2 m_previousPosition{ glm::vec2(-1.0, -1.0) };

        /**
         * The current mouse position.
         */
        glm::vec2 m_currentPosition{ glm::vec2(0.0, 0.0) };

        /**
         * The mouse displacement from the previousPos.
         */
        glm::vec2 m_displVec{ glm::vec2(0.0f, 0.0f) };

        /**
         * Info whether the mouse is in the window.
         */
        bool m_inWindow{ false };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Adds event handlers.
         */
        void InitListeners();

        //-------------------------------------------------
        // Keyboard && Mouse
        //-------------------------------------------------

        /**
         * Processes input received from the keyboard.
         *
         * @param t_direction The camera movement direction.
         */
        void ProcessKeyboard(Direction t_direction);

        /**
         * Processes input received from the mouse.
         */
        void ProcessMouse();

        //-------------------------------------------------
        // Euler angles
        //-------------------------------------------------

        /**
         * Calculates the front vector from the Camera's (updated) Euler Angles.
         */
        void UpdateCameraVectors();
    };
}
