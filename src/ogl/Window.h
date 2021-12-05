#pragma once

#include <glm/mat4x4.hpp>
#include <string>

struct GLFWwindow;

namespace sg::ogl
{
    /**
     * Encapsulate all the window initialization code.
     */
    class Window
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * The default min width of the window.
         */
        static constexpr auto MIN_WIDTH{ 640 };

        /**
         * The default min height of the window.
         */
        static constexpr auto MIN_HEIGHT{ 480 };

        /**
         * The field of view.
         */
        static constexpr auto FOV_DEG{ 60.0f };

        /**
         * The near plane of the frustum.
         */
        static constexpr auto NEAR_PLANE{ 0.01f };

        /**
         * The far plane of the frustum.
         */
        static constexpr auto FAR_PLANE{ 5000.0f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Window();
        Window(int t_width, int t_height, std::string t_title);

        Window(const Window& t_other) = delete;
        Window(Window&& t_other) noexcept = delete;
        Window& operator=(const Window& t_other) = delete;
        Window& operator=(Window&& t_other) noexcept = delete;

        ~Window() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] const std::string& GetTitle() const;
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;
        [[nodiscard]] const glm::mat4& GetProjectionMatrix() const;
        [[nodiscard]] const glm::mat4& GetOrthographicProjectionMatrix() const;
        [[nodiscard]] GLFWwindow* GetWindowHandle() const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        /**
         * Initializing class.
         */
        void Init();

        /**
         * Swaps the front and back buffers and checks if any events are triggered.
         */
        void Update();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        /**
         * Encapsulates glfwWindowShouldClose.
         *
         * @return boolean
         */
        bool WindowShouldClose();

        //-------------------------------------------------
        // Input
        //-------------------------------------------------

        /**
         * Poll the key status.
         *
         * @param keyCode A constant such as GLFW_KEY_W or GLFW_KEY_SPACE
         *
         * @return true if GLFW_PRESS
         */
        bool IsKeyPressed(int t_keyCode);

        /**
         * Close if Esc key was pressed.
         */
        void CloseIfEscKeyPressed();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The title of the window.
         */
        std::string m_title{ "SgCity" };

        /**
         * The width of the window.
         */
        int m_width{ 0 };

        /**
         * The height of the window.
         */
        int m_height{ 0 };

        /**
         * The projection matrix.
         */
        glm::mat4 m_projectionMatrix{ glm::mat4(1.0f) };

        /**
         * The orthographic projection matrix.
         */
        glm::mat4 m_orthographicProjectionMatrix{ glm::mat4(1.0f) };

        /**
         * The glfw window handle.
         */
        GLFWwindow* m_windowHandle{ nullptr };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * The Window initialization code.
         */
        void InitWindow();

        /**
         * Init projection matrices.
         */
        void InitProjectionMatrix();

        //-------------------------------------------------
        // Projection matrix
        //-------------------------------------------------

        /**
         * Update projection matrix.
         */
        void UpdateProjectionMatrix();

        /**
         * Update orthographic projection matrix.
         */
        void UpdateOrthographicProjectionMatrix();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        /**
         * Clean up.
         */
        void CleanUp() const;
    };
}
