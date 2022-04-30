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
#include "Application.h"
#include "Log.h"
#include "MenuState.h"
#include "GameState.h"
#include "state/StateStack.h"
#include "map/Map.h"
#include "ogl/OpenGL.h"
#include "city/City.h"

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

    // create && init window
    const auto ww{ INI.Get<int>("window", "width") };
    const auto wh{ INI.Get<int>("window", "height") };
    auto wt{ INI.Get<std::string>("window", "title") };
    m_window = std::make_shared<ogl::Window>(ww, wh, wt);

    // create && init camera
    const auto pos{ INI.GetVector<float>("camera", "position") };
    const auto yaw{ INI.Get<float>("camera", "yaw") };
    const auto pitch{ INI.Get<float>("camera", "pitch") };
    const auto movementSpeed{ INI.Get<float>("camera", "movement_speed") };
    const auto mouseSensitivity{ INI.Get<float>("camera", "mouse_sensitivity") };
    m_camera = std::make_unique<ogl::camera::Camera>(m_window, glm::vec3(pos[0], pos[1], pos[2]), yaw, pitch);
    m_camera->SetCameraSpeed(movementSpeed);
    m_camera->SetMouseSensitivity(mouseSensitivity);

    // create && init map tiles
    m_map = std::make_shared<map::Map>(m_window);

    // create skybox
    m_skybox = std::make_unique<ogl::resource::Skybox>();

    // create && init city
    m_city = std::make_unique<city::City>("Musterstadt", m_map);

    // create state stack
    m_stateStack = std::make_unique<state::StateStack>(state::State::Context());

    // register states
    m_stateStack->RegisterState<MenuState>(state::State::Id::MENU);
    m_stateStack->RegisterState<GameState>(state::State::Id::GAME);

    Log::SG_LOG_DEBUG("[Application::Init()] The application was successfully initialized.");
}

void sg::Application::Input() const
{
    // Esc key closes the app
    m_window->Input();

    // do nothing (return) when the mouse is over the ImGui window
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    // WASD keys + right mouse button rotation
    m_camera->Input();
}

void sg::Application::Update()
{
    m_map->Update();
    m_city->Update();
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
    m_map->RenderForMousePicking(*m_camera);

    // --------------------
    // (2) render for water
    // --------------------
    // Bind fbo
    // OpenGL::SetClearColor()
    // Clear()
    // Draw ...
    // Unbind fbo
    m_map->RenderForWater(*m_camera, *m_skybox);

    // -----------------------
    // (3) render scene && gui
    // -----------------------
    StartFrame();

    m_map->Render(*m_camera);
    m_skybox->Render(*m_window, *m_camera);

    RenderImGui();

    EndFrame();
}

void sg::Application::RenderImGui() const
{
    ogl::Window::ImGuiBegin();

    //m_camera->RenderImGui();
    m_map->RenderImGui();
    m_city->RenderImGui();

    ogl::Window::ImGuiEnd();
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

    while(!m_window->WindowShouldClose())
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
            ss << m_window->GetTitle() << " [DEBUG BUILD] " << "   |   Fps: " << fps << "   |   Updates: " << updates;
#else
            ss << m_window.GetTitle() << " [RELEASE BUILD] " << "   |   Fps: " << fps << "   |   Updates: " << updates;
#endif
            glfwSetWindowTitle(m_window->GetWindowHandle(), ss.str().c_str());

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
    m_window->SwapBuffersAndCallEvents();
}
