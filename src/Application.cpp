#include <sstream>
#include "Application.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/Texture.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/input/MouseInput.h"
#include "ogl/input/PickingTexture.h"
#include "map/Map.h"

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

    ogl::input::MouseInput::Init(m_window);

    //m_redGrid = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/texture/red.png");
    //m_redGrid->Load();

    //m_full = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/texture/full.png");
    //m_full->Load();

    m_map = std::make_unique<map::Map>();

    Log::SG_LOG_DEBUG("[Application::Init()] The application was successfully initialized.");
}

void sg::Application::Input()
{
    ogl::input::MouseInput::GetInstance().Input();

    m_window.CloseIfEscKeyPressed();

    if (ogl::input::MouseInput::GetInstance().IsRightButtonPressed())
    {
        m_camera.ProcessMouse(ogl::input::MouseInput::GetInstance().GetDisplVec());

        //Log::SG_LOG_DEBUG("Right Mouse Button pressed.");
    }

    if (ogl::input::MouseInput::GetInstance().IsLeftButtonPressed())
    {
        //Log::SG_LOG_DEBUG("Left Mouse Button pressed.");
        Log::SG_LOG_DEBUG("Id {}.", m_map->GetTileObjectId());
    }
}

void sg::Application::Update()
{
    if (m_window.IsKeyPressed(GLFW_KEY_W))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::FORWARD);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_S))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::BACKWARD);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_A))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::LEFT);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_D))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::RIGHT);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_O))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::UP);
    }

    if (m_window.IsKeyPressed(GLFW_KEY_U))
    {
        m_camera.ProcessKeyboard(ogl::camera::Camera::Direction::DOWN);
    }
}

void sg::Application::Render()
{
    m_map->RenderForMousePicking(m_window, m_camera);

    StartFrame();
    m_map->Render(m_window, m_camera);
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
    ogl::OpenGL::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ogl::OpenGL::Clear();
    ogl::OpenGL::EnableWireframeMode();
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

}
