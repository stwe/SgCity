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
#include "MenuState.h"
#include "Log.h"
#include "ogl/Window.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::MenuState::MenuState(const Id t_id, state::StateStack* t_stateStack, const Context& t_context)
    : State(t_id, t_stateStack, t_context)
{
    Log::SG_LOG_DEBUG("[MenuState::MenuState()] Create MenuState.");
}

sg::MenuState::~MenuState() noexcept
{
    Log::SG_LOG_DEBUG("[MenuState::~MenuState()] Destruct MenuState.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::MenuState::Input()
{
    // ESC ends the game
    if (context.window->IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        action = Action::EXIT;
    }

    // switch to the game state
    if (action == Action::PLAY)
    {
        Log::SG_LOG_INFO("[MenuState::Input()] Starts switching to the GameState.");
        RequestStackPop();
        RequestStackPush(Id::GAME);
    }

    // empties the stack for ending the game
    if (action == Action::EXIT)
    {
        Log::SG_LOG_INFO("[MenuState::Input()] Starts POP MenuState.");
        RequestStackPop();
    }
}

void sg::MenuState::Update()
{
}

void sg::MenuState::Render()
{
}

void sg::MenuState::RenderImGui()
{
    ogl::Window::ImGuiBegin();

    ImGui::SetNextWindowPos(
        ImVec2(static_cast<float>(context.window->GetWidth()) * 0.5f, static_cast<float>(context.window->GetHeight()) * 0.5f),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );

    ImGui::Begin("Main Menu", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoMove
    );

    for (auto i{ 1u }; i < m_buttons.size(); ++i)
    {
        if (m_buttons[i])
        {
            ImGui::PushID(static_cast<int>(i));
            ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.6f, 0.6f)));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.7f, 0.7f)));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.8f, 0.8f)));

            ImGui::Button(std::string(BUTTON_NAMES[i]).data());

            if (ImGui::IsItemClicked(0))
            {
                m_buttons[i] = !m_buttons[i];
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                ImGui::TextUnformatted(std::string(BUTTON_NAMES[i]).data());
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }

            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
        else
        {
            if (ImGui::Button(std::string(BUTTON_NAMES[i]).data()))
            {
                std::fill(m_buttons.begin(), m_buttons.end(), false);
                m_buttons[i] = true;
                action = static_cast<Action>(i);
                Log::SG_LOG_DEBUG("[MenuState::RenderImGui()] current selection: {}", BUTTON_NAMES[i]);
            }
        }
    }

    ImGui::End();

    ogl::Window::ImGuiEnd();
}
