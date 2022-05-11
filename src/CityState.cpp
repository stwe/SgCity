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
#include "CityState.h"
#include "SgAssert.h"
#include "city/City.h"
#include "ogl/resource/Skybox.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::CityState::CityState(const Id t_id, state::StateStack* t_stateStack, std::shared_ptr<Context> t_context)
    : State(t_id, t_stateStack, std::move(t_context))
{
    Log::SG_LOG_DEBUG("[CityState::CityState()] Create CityState.");

    Init();
}

sg::CityState::~CityState() noexcept
{
    Log::SG_LOG_DEBUG("[CityState::~CityState()] Destruct CityState.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::CityState::Input()
{
    // WASD keys + right mouse button rotation
    m_camera->Input();

    // switch to the MenuState
    if (action == Action::MAIN_MENU)
    {
        Log::SG_LOG_INFO("[GameState::Input()] Starts switching back to the MenuState.");
        RequestStackPop();
        RequestStackPush(Id::MAIN_MENU);
    }
}

void sg::CityState::Update()
{
    context->city->Update();
}

void sg::CityState::Render()
{
    context->city->Render(*m_camera);
    m_skybox->Render(*context->window, *m_camera);
}

void sg::CityState::RenderImGui()
{
    ogl::Window::ImGuiBegin();

    context->city->RenderImGui();

    //m_camera->RenderImGui();
    ShowMainMenuBar();
    ShowGameMenu();

    ogl::Window::ImGuiEnd();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::CityState::Init()
{
    Log::SG_LOG_DEBUG("[CityState::Init()] Initializing city state.");

    SG_ASSERT(context->window, "[CityState::Init()] Null pointer.")

    m_camera = std::make_unique<ogl::camera::Camera>(context->window);
    m_skybox = std::make_unique<ogl::resource::Skybox>();

    SG_ASSERT(context->city, "[CityState::Init()] Null pointer.")

    Log::SG_LOG_DEBUG("[CityState::Init()] The city state was successfully initialized.");
}

//-------------------------------------------------
// ImGui
//-------------------------------------------------

void sg::CityState::ShowMainMenuBar()
{
    if (m_showAbout)
    {
        ImGui::SetNextWindowPos(
            { static_cast<float>(context->window->GetWidth()) * 0.5f, static_cast<float>(context->window->GetHeight()) * 0.5f },
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

void sg::CityState::ShowGameMenu()
{
    ImGui::SetNextWindowPos(
        { static_cast<float>(context->window->GetWidth()) * 0.5f, static_cast<float>(context->window->GetHeight()) },
        ImGuiCond_Always,
        { 0.5f, 1.0f }
    );
    ImGui::SetNextWindowSize({ static_cast<float>(context->window->GetWidth()), 64.0f });
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

    ImGui::Text("City: %s", context->city->name.c_str());

    ImGui::End();
}
