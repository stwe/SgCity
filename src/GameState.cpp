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

    //m_camera->RenderImGui();

    ImGui::SetNextWindowPos(
        ImVec2(static_cast<float>(context.window->GetWidth()) * 0.5f, static_cast<float>(context.window->GetHeight())),
        ImGuiCond_Always,
        ImVec2(0.5f, 1.0f)
    );

    ImGui::Begin("Game Menu", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoMove
    );

    if (ImGui::Button("Back to Main Menu"))
    {
        action = Action::MAIN_MENU;
    }

    ImGui::End();

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
