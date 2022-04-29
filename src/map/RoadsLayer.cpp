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

#include "RoadsLayer.h"
#include "Application.h"
#include "Tile.h"
#include "Log.h"
#include "Map.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/resource/ResourceManager.h"
#include "event/EventManager.h"
#include "eventpp/utilities/argumentadapter.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::RoadsLayer::RoadsLayer(std::shared_ptr<ogl::Window> t_window, std::vector<std::shared_ptr<Tile>> t_tiles)
    : Layer(std::move(t_window), std::move(t_tiles))
{
    Log::SG_LOG_DEBUG("[RoadsLayer::RoadsLayer()] Create RoadsLayer.");

    Init();
}

sg::map::RoadsLayer::~RoadsLayer() noexcept
{
    Log::SG_LOG_DEBUG("[RoadsLayer::~RoadsLayer()] Destruct RoadsLayer.");
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::map::RoadsLayer::Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane)
{
    if (!vao)
    {
        return;
    }

    ogl::OpenGL::EnableFaceCulling();

    vao->Bind();

    const auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram(Application::RESOURCES_PATH + "shader/layer/roads") };
    shaderProgram.Bind();

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", window->GetProjectionMatrix());

    const auto& texture{ ogl::resource::ResourceManager::LoadTexture(Application::RESOURCES_PATH + "texture/roads.png") };
    texture.BindForReading(GL_TEXTURE0);
    shaderProgram.SetUniform("diffuseMap", 0);

    vao->DrawPrimitives();

    ogl::resource::ShaderProgram::Unbind();
    vao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::RoadsLayer::Init()
{
    Log::SG_LOG_DEBUG("[RoadsLayer::Init()] Initialize the RoadsLayer.");

    position = glm::vec3(0.0f);

    modelMatrix = ogl::math::Transform::CreateModelMatrix(
        position,
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    );

    InitEventDispatcher();
    CreateTiles();
    RoadTilesToGpu();

    Log::SG_LOG_DEBUG("[RoadsLayer::Init()] The RoadsLayer was successfully initialized.");
}

void sg::map::RoadsLayer::InitEventDispatcher()
{
    Log::SG_LOG_DEBUG("[RoadsLayer::InitEventDispatcher()] Append listeners.");

    // create road
    event::EventManager::eventDispatcher.appendListener(
        event::SgEventType::CREATE_ROAD,
        eventpp::argumentAdapter<void(const event::CreateRoadEvent&)>(
            [this](const event::CreateRoadEvent& t_event)
            {
                OnCreateRoad(*tiles[t_event.index]);
            }
        )
    );
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::map::RoadsLayer::CreateTiles()
{
    auto i{ 0 };
    for (const auto& tile : tiles)
    {
        if (tile->type == Tile::TileType::TRAFFIC)
        {
            m_roadTiles.emplace_back(CreateRoadTile(*tile, i));
            i++;
        }
    }
}

//-------------------------------------------------
// Listeners
//-------------------------------------------------

void sg::map::RoadsLayer::OnCreateRoad(const Tile& t_tile)
{
    Log::SG_LOG_DEBUG("[RoadsLayer::OnCreateRoad()] Built a road at {}.", t_tile.mapIndex);

    if (!CheckTerrainForRoad(t_tile))
    {
        Log::SG_LOG_DEBUG("[RoadsLayer::OnCreateRoad()] No road can be built at {}.", t_tile.mapIndex);
        return;
    }

    // create road tile
    const auto i{ static_cast<int>(m_roadTiles.size()) };
    m_roadTiles.emplace_back(CreateRoadTile(t_tile, i));

    // create a new Vao if necessary
    if (!vao)
    {
        vao = std::make_unique<ogl::buffer::Vao>();
        vao->CreateEmptyDynamicVbo(Map::TILE_COUNT * Map::TILE_COUNT * Tile::BYTES_PER_TILE, static_cast<int>(m_roadTiles.size()) * Tile::VERTICES_PER_TILE);
    }

    // store all tiles in Vao
    if (vao)
    {
        // vertices to Gpu
        for (const auto& roadTile : m_roadTiles)
        {
            UpdateTexture(*roadTile);
            roadTile->VerticesToGpu(*vao);
        }

        // update draw count
        vao->drawCount = static_cast<int>(m_roadTiles.size()) * Tile::VERTICES_PER_TILE;
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::map::RoadsLayer::RoadTilesToGpu()
{
    if (m_roadTiles.empty())
    {
        return;
    }

    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateEmptyDynamicVbo(Map::TILE_COUNT * Map::TILE_COUNT * Tile::BYTES_PER_TILE, static_cast<int>(m_roadTiles.size()) * Tile::VERTICES_PER_TILE);

    for (const auto& roadTile : m_roadTiles)
    {
        roadTile->VerticesToGpu(*vao);
    }
}

std::unique_ptr<sg::map::RoadTile> sg::map::RoadsLayer::CreateRoadTile(const Tile& t_tile, const int t_index)
{
    auto roadTile{ std::make_unique<RoadTile>() };
    roadTile->vertices = t_tile.vertices;

    roadTile->vertices[Tile::TL_1_POSITION_Y] += 0.01f;
    roadTile->vertices[Tile::BL_1_POSITION_Y] += 0.01f;
    roadTile->vertices[Tile::BR_1_POSITION_Y] += 0.01f;

    roadTile->vertices[Tile::TL_2_POSITION_Y] += 0.01f;
    roadTile->vertices[Tile::BR_2_POSITION_Y] += 0.01f;
    roadTile->vertices[Tile::TR_2_POSITION_Y] += 0.01f;

    roadTile->mapX = t_tile.mapX;
    roadTile->mapZ = t_tile.mapZ;
    roadTile->mapIndex = t_index;

    roadTile->n = t_tile.n;
    roadTile->s = t_tile.s;
    roadTile->e = t_tile.e;
    roadTile->w = t_tile.w;
    roadTile->nw = t_tile.nw;
    roadTile->ne = t_tile.ne;
    roadTile->sw = t_tile.sw;
    roadTile->se = t_tile.se;

    roadTile->type = Tile::TileType::TRAFFIC;

    UpdateTexture(*roadTile);

    return roadTile;
}

void sg::map::RoadsLayer::UpdateTexture(RoadTile& t_roadTile)
{
    t_roadTile.DetermineRoadType();

    const auto roadType{ static_cast<int>(t_roadTile.roadType) };

    const auto column{ roadType % 4 };
    const auto xOffset{ static_cast<float>(column) / 4.0f };

    const auto row{ roadType / 4 };
    const auto yOffset{ 1.0f - static_cast<float>(row) / 4.0f };

    // tl 1
    t_roadTile.vertices[Tile::TL_1_UV_X] = xOffset;
    t_roadTile.vertices[Tile::TL_1_UV_X + 1] = (1.0f / 4.0f) + yOffset;

    // bl
    t_roadTile.vertices[Tile::BL_1_UV_X] = xOffset;
    t_roadTile.vertices[Tile::BL_1_UV_X + 1] = yOffset;

    // br 1
    t_roadTile.vertices[Tile::BR_1_UV_X] = (1.0f / 4.0f) + xOffset;
    t_roadTile.vertices[Tile::BR_1_UV_X + 1] = yOffset;

    // tl 2
    t_roadTile.vertices[Tile::TL_2_UV_X] = xOffset;
    t_roadTile.vertices[Tile::TL_2_UV_X + 1] = (1.0f / 4.0f) + yOffset;

    // br 2
    t_roadTile.vertices[Tile::BR_2_UV_X] = (1.0f / 4.0f) + xOffset;
    t_roadTile.vertices[Tile::BR_2_UV_X + 1] = yOffset;

    // tr
    t_roadTile.vertices[Tile::TR_2_UV_X] = (1.0f / 4.0f) + xOffset;
    t_roadTile.vertices[Tile::TR_2_UV_X + 1] = (1.0f / 4.0f) + yOffset;
}

bool sg::map::RoadsLayer::CheckTerrainForRoad(const Tile& t_tile)
{
    for (const auto idx : Tile::Y_INDEX)
    {
        if (t_tile.vertices[idx] < 0.0f)
        {
            return false;
        }
    }

    if (t_tile.vertices[Tile::TL_1_POSITION_Y] > t_tile.vertices[Tile::BL_1_POSITION_Y] &&
        t_tile.vertices[Tile::TR_2_POSITION_Y] <= t_tile.vertices[Tile::BR_1_POSITION_Y])
    {
        return false;
    }

    if (t_tile.vertices[Tile::TL_1_POSITION_Y] <= t_tile.vertices[Tile::BL_1_POSITION_Y] &&
        t_tile.vertices[Tile::TR_2_POSITION_Y] > t_tile.vertices[Tile::BR_1_POSITION_Y])
    {
        return false;
    }

    if (t_tile.vertices[Tile::BL_1_POSITION_Y] > t_tile.vertices[Tile::TL_1_POSITION_Y] &&
        t_tile.vertices[Tile::BR_1_POSITION_Y] <= t_tile.vertices[Tile::TR_2_POSITION_Y])
    {
        return false;
    }

    if (t_tile.vertices[Tile::BL_1_POSITION_Y] <= t_tile.vertices[Tile::TL_1_POSITION_Y] &&
        t_tile.vertices[Tile::BR_1_POSITION_Y] > t_tile.vertices[Tile::TR_2_POSITION_Y])
    {
        return false;
    }

    return true;
}
