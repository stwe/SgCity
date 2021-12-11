#pragma once

#include "ogl/Window.h"

namespace sg::ogl::input
{
    class MouseInput
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MouseInput(const MouseInput& t_other) = delete;
        MouseInput(MouseInput&& t_other) noexcept = delete;
        MouseInput& operator=(const MouseInput& t_other) = delete;
        MouseInput& operator=(MouseInput&& t_other) noexcept = delete;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] auto GetX() const { return static_cast<int>(m_currentPosition.x); }
        [[nodiscard]] auto GetY() const { return static_cast<int>(m_currentPosition.y); }

        [[nodiscard]] auto GetCurrentPosition() const { return m_currentPosition; }
        [[nodiscard]] auto GetDisplVec() const { return m_displVec; }
        [[nodiscard]] auto IsInWindow() const { return m_inWindow; }
        [[nodiscard]] bool IsLeftButtonPressed() const { return m_leftButtonPressed; }
        [[nodiscard]] bool IsRightButtonPressed() const { return m_rightButtonPressed; }

        //-------------------------------------------------
        // Singleton
        //-------------------------------------------------

        static MouseInput& GetInstance();

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        static void Init(const Window& t_window);
        void Input();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The previous position.
         */
        glm::dvec2 m_previousPosition{ glm::dvec2(-1.0, -1.0) };

        /**
         * The current position.
         */
        glm::dvec2 m_currentPosition{ glm::dvec2(0.0, 0.0) };

        /**
         * The mouse displacement from the previousPos.
         */
        glm::vec2 m_displVec{ glm::vec2(0.0f, 0.0f) };

        /**
         * Info whether the mouse is in the window.
         */
        bool m_inWindow{ false };

        /**
         * Flag whether the left mouse button was pressed.
         */
        bool m_leftButtonPressed{ false };

        /**
         * Flag whether the right mouse button was pressed.
         */
        bool m_rightButtonPressed{ false };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        MouseInput() = default;
        ~MouseInput() noexcept = default;
    };
}
