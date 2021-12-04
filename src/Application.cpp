#include <sstream>
#include "Application.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/resource/Texture.h"
#include "ogl/math/Transform.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::Application::Application()
{
    Log::SG_LOG_DEBUG("[Application::Application()] Create Application.");
}

sg::Application::~Application() noexcept
{
    Log::SG_LOG_DEBUG("[Application::~Application()] Destruct Application.");

    CleanUp();
}

//-------------------------------------------------
// Run
//-------------------------------------------------

void sg::Application::Run()
{
    Log::SG_LOG_DEBUG("[Application::Run()] Starts running application.");

    Init();
    GameLoop();

    Log::SG_LOG_DEBUG("[Application::Run()] Goodbye ...");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::Application::Init()
{
    Log::SG_LOG_DEBUG("[Application::Init()] Initializing application.");

    m_window.Init();

    m_vao = new ogl::buffer::Vao();
    m_shaderProgram = new ogl::resource::ShaderProgram("/home/steffen/CLionProjects/SgCity/resources/shader/sprite");
    m_texture = new ogl::resource::Texture("/home/steffen/CLionProjects/SgCity/resources/texture/t.png");

    m_vao->Add2DQuadVbo();
    m_shaderProgram->Load();
    m_texture->Load();

    Log::SG_LOG_DEBUG("[Application::Init()] The application was successfully initialized.");
}

void sg::Application::Input()
{
    m_window.CloseIfEscKeyPressed();
}

void sg::Application::Update()
{

}

void sg::Application::Render()
{
    StartFrame();

    ogl::OpenGL::EnableAlphaBlending();

    m_vao->Bind();
    m_shaderProgram->Bind();

    m_shaderProgram->SetUniform("model", ogl::math::Transform::CreateModelMatrix(
        glm::vec2(200.0f, 200.0f), glm::vec2(200.0f, 200.0f)));
    m_shaderProgram->SetUniform("view", m_camera.GetViewMatrix());
    m_shaderProgram->SetUniform("projection", m_window.GetOrthographicProjectionMatrix());

    m_texture->BindForReading(GL_TEXTURE0);
    m_shaderProgram->SetUniform("diffuseMap", 0);

    m_vao->DrawPrimitives();

    m_shaderProgram->Unbind();
    m_vao->Unbind();

    ogl::OpenGL::DisableBlending();

    EndFrame();
}

//-------------------------------------------------
// Game loop
//-------------------------------------------------

void sg::Application::GameLoop()
{
    Log::SG_LOG_DEBUG("[Application::GameLoop()] Starting the game loop.");

    auto lastTime{ glfwGetTime() };
    auto resetTimer{ lastTime };

    auto dt{ 0.0 };
    auto fps{ 0 };
    auto updates{ 0 };

    while(!m_window.WindowShouldClose())
    {
        auto now{ glfwGetTime() };
        dt += (now - lastTime) / FRAME_TIME;
        lastTime = now;

        Input();

        while (dt >= 1.0)
        {
            Update();
            updates++;
            dt--;
        }

        Render();
        fps++;

        if (glfwGetTime() - resetTimer > 1.0)
        {
            resetTimer++;

            std::stringstream ss;
#ifdef SG_CITY_DEBUG_BUILD
            ss << m_window.GetTitle() << " [DEBUG BUILD] " << "   |   Fps: " << fps << "   |   Updates: " << updates;
#else
            ss << m_window.GetTitle() << " [RELEASE BUILD] " << "   |   Fps: " << fps << "   |   Updates: " << updates;
#endif
            glfwSetWindowTitle(m_window.GetWindowHandle(), ss.str().c_str());

            updates = 0;
            fps = 0;
        }
    }
}

//-------------------------------------------------
// Frame
//-------------------------------------------------

void sg::Application::StartFrame()
{
    sg::ogl::OpenGL::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    sg::ogl::OpenGL::Clear();
}

void sg::Application::EndFrame()
{
    m_window.Update();
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::Application::CleanUp()
{
    delete m_vao;
    delete m_shaderProgram;
    delete m_texture;
}
