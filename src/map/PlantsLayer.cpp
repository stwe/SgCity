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

#include "PlantsLayer.h"
#include "Tile.h"
#include "Log.h"
#include "ogl/resource/ResourceManager.h"
#include "ogl/resource/Model.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::PlantsLayer::PlantsLayer(std::vector<std::shared_ptr<Tile>> t_tiles)
    : Layer(std::move(t_tiles))
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

void sg::map::PlantsLayer::Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const
{
    for (const auto& tile : tiles)
    {
        if (tile->type == Tile::TileType::PLANTS)
        {
            m_model->Render(t_window, t_camera, glm::vec3(tile->mapX + 0.5f, 0.001f, tile->mapZ + 0.5f));
        }
    }
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::PlantsLayer::Init()
{
    Log::SG_LOG_DEBUG("[PlantsLayer::Init()] Initialize the PlantsLayer.");

    m_model = ogl::resource::ResourceManager::LoadModel("E:/Dev/SgCity/resources/model/Tree_02/billboardmodel.obj");

    Log::SG_LOG_DEBUG("[PlantsLayer::Init()] The PlantsLayer was successfully initialized.");
}
