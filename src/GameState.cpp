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

#include <imgui.h>
#include "GameState.h"
#include "Log.h"
#include "ogl/resource/Skybox.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::GameState::GameState(const Id t_id, state::StateStack* t_stateStack, const Context& t_context)
    : State(t_id, t_stateStack, t_context)
{
    Log::SG_LOG_DEBUG("[GameState::GameState()] Create GameState.");

    Init();
}

sg::GameState::~GameState() noexcept
{
    Log::SG_LOG_DEBUG("[GameState::~GameState()] Destruct GameState.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::GameState::Input()
{
    // ESC ecs ends the state
    // todo ESC beendet alle States
    if (context.window->IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        action = Action::MAIN_MENU;
    }

    // WASD keys + right mouse button rotation
    m_camera->Input();

    // switch to the menu state
    if (action == Action::MAIN_MENU)
    {
        Log::SG_LOG_INFO("[GameState::Input()] Starts switching back to the MenuState.");
        RequestStackPop();
        RequestStackPush(Id::MENU);
    }
}

void sg::GameState::Update()
{
}

void sg::GameState::Render()
{
    m_skybox->Render(*context.window, *m_camera);
}

void sg::GameState::RenderImGui()
{
    ogl::Window::ImGuiBegin();

    if (m_initialRun)
    {
        ImGui::SetNextWindowPos(
            { static_cast<float>(context.window->GetWidth()) * 0.5f, static_cast<float>(context.window->GetHeight()) * 0.5f },
            ImGuiCond_Always,
            { 0.5f, 0.5f }
        );
        ImGui::Begin("New City", &m_initialRun,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoMove
            );

        ImGui::InputText("Name", m_cityName, IM_ARRAYSIZE(m_cityName));

        static int e{ 0 };
        ImGui::RadioButton("Easy (20.000 EUR)", &e, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Medium (10.000 EUR)", &e, 1);
        ImGui::SameLine();
        ImGui::RadioButton("Hard (10k Bonds)", &e, 2);
        ImGui::Separator();

        if (ImGui::Button("Done"))
        {
            m_initialRun = false;
            if (strlen(m_cityName) <= 4)
            {
                strncpy(m_cityName, "Musterstadt", 96);
            }
            m_level = static_cast<Level>(e);
        }

        ImGui::End();
    }
    else
    {
        //m_camera->RenderImGui();
        ShowMainMenuBar();
        ShowGameMenu();
    }

    ogl::Window::ImGuiEnd();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::GameState::Init()
{
    Log::SG_LOG_DEBUG("[GameState::Init()] Initializing game state.");

    m_camera = std::make_unique<ogl::camera::Camera>(context.window);
    m_skybox = std::make_unique<ogl::resource::Skybox>();

    Log::SG_LOG_DEBUG("[GameState::Init()] The game state was successfully initialized.");
}

//-------------------------------------------------
// ImGui
//-------------------------------------------------

void sg::GameState::ShowMainMenuBar()
{
    if (m_showAbout)
    {
        ImGui::SetNextWindowPos(
            { static_cast<float>(context.window->GetWidth()) * 0.5f, static_cast<float>(context.window->GetHeight()) * 0.5f },
            ImGuiCond_Appearing,
            { 0.5f, 0.5f }
        );
        ImGui::Begin("About SgCity", &m_showAbout, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

        ImGui::Text("SgCity");
        ImGui::Separator();
        ImGui::Text("Copyright (c) 2022. stwe <https://github.com/stwe/SgCity>");

        ImGui::End();
    }

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load City")) {}
            if (ImGui::MenuItem("New City")) {}
            if (ImGui::MenuItem("Edit New Map")) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Save City")) {}
            if (ImGui::MenuItem("Save City As...")) {}

            ImGui::Separator();

            if (ImGui::MenuItem("About", nullptr, &m_showAbout)) {}

            ImGui::Separator();

            if (ImGui::MenuItem("Quit"))
            {
                action = Action::MAIN_MENU;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Speed"))
        {
            if (ImGui::MenuItem("Pause")) {}
            if (ImGui::MenuItem("Turtle")) {}
            if (ImGui::MenuItem("Llama")) {}
            if (ImGui::MenuItem("Cheetah")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {
            if (ImGui::MenuItem("Budget")) {}
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void sg::GameState::ShowGameMenu()
{
    ImGui::SetNextWindowPos(
        { static_cast<float>(context.window->GetWidth()) * 0.5f, static_cast<float>(context.window->GetHeight()) },
        ImGuiCond_Always,
        { 0.5f, 1.0f }
    );
    ImGui::SetNextWindowSize({ static_cast<float>(context.window->GetWidth()), 64.0f });
    ImGui::Begin("Game Menu", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoMove
    );

    if (ImGui::Button("Test"))
    {
    }
    ImGui::SameLine();
    if (!m_initialRun)
    {
        ImGui::Text("City: %s", m_cityName);
        ImGui::Text("Level: %i", m_level);
    }

    ImGui::End();
}
