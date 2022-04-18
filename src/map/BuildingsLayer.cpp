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
#include "BuildingsLayer.h"
#include "Tile.h"
#include "Log.h"
#include "ogl/resource/ResourceManager.h"
#include "ogl/resource/Model.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::BuildingsLayer::BuildingsLayer(std::shared_ptr<ogl::Window> t_window, std::vector<std::shared_ptr<Tile>> t_tiles)
    : Layer(std::move(t_window), std::move(t_tiles))
{
    Log::SG_LOG_DEBUG("[BuildingsLayer::BuildingsLayer()] Create BuildingsLayer.");

    Init();
}

sg::map::BuildingsLayer::~BuildingsLayer() noexcept
{
    Log::SG_LOG_DEBUG("[BuildingsLayer::~BuildingsLayer()] Destruct BuildingsLayer.");
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::map::BuildingsLayer::Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane)
{
    m_render = 0;
    m_skip = 0;

    // todo: cache models
    for (const auto& tile : tiles)
    {
        if (tile->type == Tile::TileType::RESIDENTIAL)
        {
            auto position{ glm::vec3(tile->mapX + 0.5f, 0.001f, tile->mapZ + 0.5f) };

            if (!m_model->sphereVolume.IsOnFrustum(t_camera.GetCurrentFrustum(), position))
            {
                m_skip++;
                continue;
            }

            m_model->Render(t_camera, position);

            m_render++;
        }
    }
}

void sg::map::BuildingsLayer::RenderImGui()
{
    ImGui::Begin("Buildings Layer");
    ImGui::Text("Rendered buildings: %d", m_render);
    ImGui::Text("Skipped buildings: %d", m_skip);
    ImGui::End();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::BuildingsLayer::Init()
{
    Log::SG_LOG_DEBUG("[BuildingsLayer::Init()] Initialize the BuildingsLayer.");

    m_model = ogl::resource::ResourceManager::LoadModel(window, "E:/Dev/SgCity/resources/model/node_115.obj");

    Log::SG_LOG_DEBUG("[BuildingsLayer::Init()] The BuildingsLayer was successfully initialized.");
}
