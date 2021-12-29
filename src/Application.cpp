#include <sstream>
#include <imgui.h>
#include "Application.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/resource/Model.h"
#include "ogl/input/MouseInput.h"
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

    // init window
    m_window.Init();

    // init mouse
    ogl::input::MouseInput::Init(m_window);

    // create && init map tiles
    m_map = std::make_unique<map::Map>(TILE_COUNT);

    Log::SG_LOG_DEBUG("[Application::Init()] The application was successfully initialized.");
}

void sg::Application::Input()
{
    // Esc key closes the app
    m_window.CloseIfEscKeyPressed();

    // do nothing when the mouse is over the ImGui window
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    // updated mouse stuff
    ogl::input::MouseInput::GetInstance().Input();

    // handle right mouse button
    if (ogl::input::MouseInput::GetInstance().IsRightButtonPressed())
    {
        m_camera.ProcessMouse(ogl::input::MouseInput::GetInstance().GetDisplVec());
    }

    // workaround: handle *not* left mouse button
    if (!ogl::input::MouseInput::GetInstance().IsLeftButtonPressed())
    {
        m_handleMouseEvent = true;
    }

    // handle left mouse button
    if (ogl::input::MouseInput::GetInstance().IsLeftButtonPressed() && m_handleMouseEvent)
    {
        m_map->Update(m_mapEditGui.raise);

        // do not run the event again
        m_handleMouseEvent = false;
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

    if (m_window.IsKeyPressed(GLFW_KEY_I))
    {
        Log::SG_LOG_DEBUG("Camera position: x: {}, y: {}, z: {}, yaw: {}, pitch: {}",
                          m_camera.GetPosition().x,
                          m_camera.GetPosition().y,
                          m_camera.GetPosition().z,
                          m_camera.GetYaw(),
                          m_camera.GetPitch()
                          );
    }
}

void sg::Application::Render()
{
    // (1) render for mousepicking
    m_map->RenderForMousePicking(m_window, m_camera);

    StartFrame();

    // (2) render scene
    m_map->Render(m_window, m_camera);
    //m_roadNetwork->Render(m_window, m_camera);
    //m_treeModel->Render(m_window, m_camera);

    // (3) render gui
    ogl::Window::ImGuiBegin();
    m_mapEditGui.Render();
    ogl::Window::ImGuiEnd();

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
    //ogl::OpenGL::EnableWireframeMode();
}

void sg::Application::EndFrame()
{
    m_window.Update();
    //ogl::OpenGL::DisableWireframeMode();
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::Application::CleanUp()
{
    Log::SG_LOG_DEBUG("[Application::CleanUp()] Clean up Application.");

    // ...
}
