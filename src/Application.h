#pragma once

#include <memory>
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"

namespace sg::ogl::resource
{
    class Texture;
}

namespace sg::ogl::renderer
{
    class SpriteRenderer;
}

namespace sg
{
    /**
     * Represents the main application class of the game.
     */
    class Application
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        static constexpr auto FRAME_TIME{ 1.0 / 60.0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Application();

        Application(const Application& t_other) = delete;
        Application(Application&& t_other) noexcept = delete;
        Application& operator=(const Application& t_other) = delete;
        Application& operator=(Application&& t_other) noexcept = delete;

        ~Application() noexcept;

        //-------------------------------------------------
        // Run
        //-------------------------------------------------

        void Run();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        ogl::Window m_window{ 1024, 768, "SgCity Sandbox" };
        ogl::camera::Camera m_camera{ glm::vec3(75.0f, 38.0f, -71.0f), 111.0f, -23.0f };

        std::unique_ptr<ogl::resource::Texture> m_redGrid;
        std::unique_ptr<ogl::resource::Texture> m_full;

        std::unique_ptr<ogl::renderer::SpriteRenderer> m_spriteRenderer;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Init();
        void Input();
        void Update();
        void Render();

        //-------------------------------------------------
        // Game loop
        //-------------------------------------------------

        void GameLoop();

        //-------------------------------------------------
        // Frame
        //-------------------------------------------------

        void StartFrame();
        void EndFrame();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
