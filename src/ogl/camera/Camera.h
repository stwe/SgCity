#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

namespace sg::ogl::camera
{
    class Camera
    {
    public:
        //-------------------------------------------------
        // Type
        //-------------------------------------------------

        enum class Direction { LEFT, RIGHT, UP, DOWN };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        glm::vec2 position{ 0.0f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Camera() = delete;
        explicit Camera(const glm::vec2& t_position);

        Camera(const Camera& t_other) = delete;
        Camera(Camera&& t_other) noexcept = delete;
        Camera& operator=(const Camera& t_other) = delete;
        Camera& operator=(Camera&& t_other) noexcept = delete;

        ~Camera() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] glm::mat4 GetViewMatrix() const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();

    protected:

    private:
        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void HandleKeyInput();
        void HandleMouseInput();
    };
}
