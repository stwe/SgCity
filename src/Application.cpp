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

#include <sstream>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "Application.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/input/MouseInput.h"
#include "ogl/resource/Skybox.h"
#include "map/Map.h"
#include "gui/MapEditGui.h"

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

    // init mouse - register callbacks
    ogl::input::MouseInput::Init(m_window);

    // create && init map tiles
    m_map = std::make_unique<map::Map>(TILE_COUNT);

    // create gui
    m_mapEditGui = std::make_unique<gui::MapEditGui>();

    // create skybox
    m_skybox = std::make_unique<ogl::resource::Skybox>();

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

    InputRightMouseButton();
    InputLeftMouseButton();
    InputKey();
}

void sg::Application::Update()
{
    m_map->Update(m_mapEditGui->action);
}

void sg::Application::Render()
{
    // ---------------------------
    // (1) render for mousepicking
    // ---------------------------
    // Bind fbo
    // OpenGL::SetClearColor()
    // Clear()
    // Draw ...
    // Unbind fbo
    m_map->RenderForMousePicking(m_window, m_camera);

    // --------------------
    // (2) render for water
    // --------------------
    // Bind fbo
    // OpenGL::SetClearColor()
    // Clear()
    // Draw ...
    // Unbind fbo
    m_map->RenderForWater(m_window, m_camera, *m_skybox);

    // -----------------------
    // (3) render scene && gui
    // -----------------------
    StartFrame();

    m_map->Render(m_window, m_camera);
    m_skybox->Render(m_window, m_camera);

    ogl::Window::ImGuiBegin();

    RenderImGui();
    m_mapEditGui->RenderImGui(*m_map);
    m_map->RenderImGui();
    ogl::Window::ImGuiEnd();

    EndFrame();
}

void sg::Application::RenderImGui()
{
    ImGui::Begin("Application");

    ImGui::SliderFloat3("Camera", value_ptr(m_camera.GetPosition()), -100.0f, 100.0f);

    ImGui::End();
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
        const auto now{ glfwGetTime() };
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
}

void sg::Application::EndFrame() const
{
    m_window.Update();
}

//-------------------------------------------------
// Logic helper
//-------------------------------------------------

void sg::Application::InputLeftMouseButton()
{
    // workaround: handle *not* left mouse button
    if (!ogl::input::MouseInput::GetInstance().IsLeftButtonPressed())
    {
        m_handleMouseEvent = true;
    }

    if (ogl::input::MouseInput::GetInstance().IsLeftButtonPressed() && m_handleMouseEvent)
    {
        m_map->Input();
        m_handleMouseEvent = false;
    }
}

void sg::Application::InputRightMouseButton()
{
    if (ogl::input::MouseInput::GetInstance().IsRightButtonPressed())
    {
        m_camera.ProcessMouse(ogl::input::MouseInput::GetInstance().GetDisplVec());
    }
}

void sg::Application::InputKey()
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
