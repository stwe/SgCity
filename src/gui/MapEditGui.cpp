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

#include "MapEditGui.h"
#include "Log.h"
#include "ogl/resource/ResourceManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::gui::MapEditGui::MapEditGui()
{
    Log::SG_LOG_DEBUG("[MapEditGui::MapEditGui()] Create MapEditGui.");

    m_buttonTextures = CreateButtonTextures();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::gui::MapEditGui::RenderImGui()
{
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("Map Edit");
    ImGui::PopStyleColor();

    for (auto i{ 0u }; i < m_buttons.size(); ++i)
    {
        ImVec2 uv1;
        uv1.x = 1.0f;
        uv1.y = 1.0f;
        if (i >= 2 && i <= 4)
        {
            uv1.y = -1.0f;
        }

        if (m_buttons[i])
        {
            ImGui::PushID(static_cast<int>(i));
            ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.6f, 0.6f)));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.7f, 0.7f)));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.8f, 0.8f)));

            ImGui::ImageButton(m_buttonTextures[i], SIZE, UV0, uv1, FRAME_PADDING, BG, TINT);
            //ImGui::Button(m_buttonNames[i].c_str());
            if (ImGui::IsItemClicked(0))
            {
                m_buttons[i] = !m_buttons[i];
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                ImGui::TextUnformatted(BUTTON_NAMES[i].c_str());
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }

            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
        else
        {
            if(ImGui::ImageButton(m_buttonTextures[i], SIZE, UV0, uv1, FRAME_PADDING, BG, TINT))
            //if (ImGui::Button(m_buttonNames[i].c_str()))
            {
                std::fill(m_buttons.begin(), m_buttons.end(), false);
                m_buttons[i] = true;
                action = static_cast<Action>(i);
                Log::SG_LOG_DEBUG("[MapEditGui::RenderImGui()] Set current action to: {}", BUTTON_NAMES[i]);
            }
        }

        ImGui::SameLine();
    }

    ImGui::NewLine();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

std::vector<ImTextureID> sg::gui::MapEditGui::CreateButtonTextures()
{
    Log::SG_LOG_DEBUG("[MapEditGui::CreateButtonTextures()] Load textures.");

    std::vector<ImTextureID> textures;

    const auto& raiseTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/raise.png") };
    const auto& lowerTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/lower.png") };
    const auto& rTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/r.png", true) };
    const auto& cTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/c.png") };
    const auto& iTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/i.png") };
    const auto& tTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/traffic.png") };
    const auto& plantTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/plants.png") };
    const auto& infoTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/info.png") };

    textures.push_back(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(raiseTexture.id)));
    textures.push_back(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(lowerTexture.id)));
    textures.push_back(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(rTexture.id)));
    textures.push_back(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(cTexture.id)));
    textures.push_back(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(iTexture.id)));
    textures.push_back(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(tTexture.id)));
    textures.push_back(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(plantTexture.id)));
    textures.push_back(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(infoTexture.id)));

    return textures;
}
