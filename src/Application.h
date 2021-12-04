#pragma once

#include "ogl/Window.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/resource/Texture.h"
#include "ogl/camera/Camera.h"

namespace sg
{
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
        ogl::Window m_window{ 1024, 768, "SgCity Sandbox" };
        ogl::camera::Camera m_camera{ glm::vec2(0.0f, 0.0f) };
        ogl::buffer::Vao* m_vao{ nullptr };
        ogl::resource::ShaderProgram* m_shaderProgram{ nullptr };
        ogl::resource::Texture* m_texture{ nullptr };

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
