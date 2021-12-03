#pragma once

#include "Window.h"
#include "buffer/Vao.h"
#include "resource/ShaderProgram.h"

namespace sg
{
    class Application
    {
    public:

        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * Time per frame.
         */
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
        ogl::buffer::Vao m_vao;
        ogl::resource::ShaderProgram m_shaderProgram{"/home/steffen/CLionProjects/SgCity/resources/shader/sprite"};

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
    };
}
