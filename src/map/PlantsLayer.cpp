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
#include "PlantsLayer.h"
#include "Tile.h"
#include "Log.h"
#include "ogl/primitives/Sphere.h"
#include "ogl/resource/ResourceManager.h"
#include "ogl/resource/Model.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::PlantsLayer::PlantsLayer(std::shared_ptr<ogl::Window> t_window, std::vector<std::shared_ptr<Tile>> t_tiles)
    : Layer(std::move(t_window), std::move(t_tiles))
{
    Log::SG_LOG_DEBUG("[PlantsLayer::PlantsLayer()] Create PlantsLayer.");

    Init();
}

sg::map::PlantsLayer::~PlantsLayer() noexcept
{
    Log::SG_LOG_DEBUG("[PlantsLayer::~PlantsLayer()] Destruct PlantsLayer.");
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::map::PlantsLayer::Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane)
{
    m_render = 0;
    m_skip = 0;

    // todo: cache models
    // todo: plane
    for (const auto& tile : tiles)
    {
        if (tile->type == Tile::TileType::PLANTS)
        {
            auto position{ glm::vec3(tile->mapX + 0.5f, 0.0f, tile->mapZ + 0.5f) };
            auto rotation{ glm::vec3(0.0f, 0.0f, 0.0f) };
            auto scale{ glm::vec3(1.0f) };

            if (!m_model->sphereVolume.IsOnFrustum(t_camera.GetCurrentFrustum(), position, rotation, scale))
            {
                m_skip++;
                continue;
            }

            m_model->Render(t_camera, position, rotation, scale);

            const glm::vec3 transformMatrix{
                ogl::math::Transform::CreateModelMatrix(
                position, rotation, scale) * glm::vec4(m_model->sphereVolume.center, 1.0f)
            };

            m_model->sphere->Render(t_camera, transformMatrix);

            m_render++;
        }
    }
}

void sg::map::PlantsLayer::RenderImGui()
{
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("Plants Layer");
    ImGui::PopStyleColor();

    ImGui::Text("Rendered plants: %d", m_render);
    ImGui::Text("Skipped plants: %d", m_skip);
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::PlantsLayer::Init()
{
    Log::SG_LOG_DEBUG("[PlantsLayer::Init()] Initialize the PlantsLayer.");

    m_model = ogl::resource::ResourceManager::LoadModel(window, "E:/Dev/SgCity/resources/model/tree/prop_001_pine.obj");

    Log::SG_LOG_DEBUG("[PlantsLayer::Init()] The PlantsLayer was successfully initialized.");
}
