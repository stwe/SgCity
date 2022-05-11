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
#include "StartState.h"
#include "Log.h"
#include "city/City.h"
#include "map/Map.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::StartState::StartState(const Id t_id, state::StateStack* t_stateStack, std::shared_ptr<Context> t_context)
    : State(t_id, t_stateStack, std::move(t_context))
{
    Log::SG_LOG_DEBUG("[StartState::StartState()] Create StartState.");
}

sg::StartState::~StartState() noexcept
{
    Log::SG_LOG_DEBUG("[StartState::~StartState()] Destruct StartState.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::StartState::Input()
{
    // ESC for main menu
    // todo ESC beendet alle States
    if (context->window->IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        action = Action::MAIN_MENU;
    }

    // switch back to the menu state
    if (action == Action::MAIN_MENU)
    {
        Log::SG_LOG_INFO("[StartState::Input()] Starts switching back to the MenuState.");
        RequestStackPop();
        RequestStackPush(Id::MAIN_MENU);
    }

    // switch to the city state
    if (action == Action::CREATE_CITY)
    {
        Log::SG_LOG_INFO("[StartState::Input()] Starts switching to the CityState.");
        Log::SG_LOG_INFO("[StartState::Input()] Name: {}, Level: {}, Size: {}", m_cityName, m_level, m_mapSize);

        // create a map and a city
        auto map{ std::make_unique<map::Map>(m_mapSize, context->window) };
        context->city = std::make_unique<city::City>(m_cityName, std::move(map));

        RequestStackPop();
        RequestStackPush(Id::CITY);
    }
}

void sg::StartState::Update()
{
}

void sg::StartState::Render()
{
}

void sg::StartState::RenderImGui()
{
    ogl::Window::ImGuiBegin();

    ImGui::SetNextWindowPos(
        { static_cast<float>(context->window->GetWidth()) * 0.5f, static_cast<float>(context->window->GetHeight()) * 0.5f },
        ImGuiCond_Always,
        { 0.5f, 0.5f }
    );
    ImGui::Begin("New City", nullptr,
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

    static int s{ 0 };
    ImGui::RadioButton("Small (128x128)", &s, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Medium (256x256)", &s, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Huge (512x512)", &s, 2);
    ImGui::Separator();

    if (ImGui::Button("Done"))
    {
        if (strlen(m_cityName) <= 4)
        {
            strncpy(m_cityName, "Musterstadt", 96);
        }
        m_mapSize = MAP_SIZES.at(s);
        m_level = static_cast<Level>(e);

        action = Action::CREATE_CITY;
    }

    ImGui::End();

    ogl::Window::ImGuiEnd();
}
