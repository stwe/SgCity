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

#include <glm/gtc/matrix_inverse.hpp>
#include "RoadsLayer.h"
#include "Tile.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/resource/ResourceManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::RoadsLayer::RoadsLayer(std::vector<std::shared_ptr<Tile>> t_tiles)
    : Layer(std::move(t_tiles))
{
    Log::SG_LOG_DEBUG("[RoadsLayer::RoadsLayer()] Create RoadsLayer.");

    Init();
}

sg::map::RoadsLayer::~RoadsLayer() noexcept
{
    Log::SG_LOG_DEBUG("[RoadsLayer::~RoadsLayer()] Destruct RoadsLayer.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::RoadsLayer::Update(gui::Action t_action, const int t_tileIndex)
{
    auto& terrainTile{ *tiles[t_tileIndex] };

    // return if TileType is already TRAFFIC
    if (terrainTile.type == Tile::TileType::TRAFFIC)
    {
        return;
    }

    Log::SG_LOG_DEBUG("[RoadsLayer::Update()] Built a road at {}.", t_tileIndex);

    if (!CheckTerrainForRoad(terrainTile))
    {
        Log::SG_LOG_DEBUG("[RoadsLayer::Update()] No road can be built at {}.", t_tileIndex);
        return;
    }

    // create road tile
    auto i{ static_cast<int>(m_roadTiles.size()) };
    m_roadTiles.push_back(std::move(CreateRoadTile(terrainTile, i)));

    // set TRAFFIC TileType
    terrainTile.type = Tile::TileType::TRAFFIC;

    // create a new Vao if necessary
    if (!vao)
    {
        vao = std::make_unique<ogl::buffer::Vao>();
        vao->CreateEmptyDynamicVbo(tileCount * tileCount * Tile::BYTES_PER_TILE, static_cast<int>(m_roadTiles.size()) * Tile::VERTICES_PER_TILE);
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
// Override
//-------------------------------------------------

void sg::map::RoadsLayer::Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const
{
    if (!vao)
    {
        return;
    }

    ogl::OpenGL::EnableFaceCulling();

    vao->Bind();

    auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram("E:/Dev/SgCity/resources/shader/map") };
    shaderProgram.Bind();

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", t_window.GetProjectionMatrix());

    const auto mv{ t_camera.GetViewMatrix() * modelMatrix };
    const auto n{ glm::inverseTranspose(glm::mat3(mv)) };
    shaderProgram.SetUniform("normalMatrix", n);

    const auto& texture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/roads.png") };
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

    CreateTiles();
    RoadTilesToGpu();

    Log::SG_LOG_DEBUG("[RoadsLayer::Init()] The RoadsLayer was successfully initialized.");
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
            m_roadTiles.push_back(std::move(CreateRoadTile(*tile, i)));
            i++;
        }
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
    vao->CreateEmptyDynamicVbo(tileCount * tileCount * Tile::BYTES_PER_TILE, static_cast<int>(m_roadTiles.size()) * Tile::VERTICES_PER_TILE);

    for (const auto& roadTile : m_roadTiles)
    {
        roadTile->VerticesToGpu(*vao);
    }
}

std::unique_ptr<sg::map::RoadTile> sg::map::RoadsLayer::CreateRoadTile(const sg::map::Tile& t_tile, const int t_index)
{
    auto roadTile{ std::make_unique<RoadTile>() };
    roadTile->vertices = t_tile.vertices;

    roadTile->vertices[Tile::TL_1_Y] += 0.01f;
    roadTile->vertices[Tile::BL_1_Y] += 0.01f;
    roadTile->vertices[Tile::BR_1_Y] += 0.01f;

    roadTile->vertices[Tile::TL_2_Y] += 0.01f;
    roadTile->vertices[Tile::BR_2_Y] += 0.01f;
    roadTile->vertices[Tile::TR_2_Y] += 0.01f;

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
    t_roadTile.vertices[3] = xOffset;
    t_roadTile.vertices[4] = (1.0f / 4.0f) + yOffset;

    // bl
    t_roadTile.vertices[14] = xOffset;
    t_roadTile.vertices[15] = yOffset;

    // br 1
    t_roadTile.vertices[25] = (1.0f / 4.0f) + xOffset;
    t_roadTile.vertices[26] = yOffset;

    // tl 2
    t_roadTile.vertices[36] = xOffset;
    t_roadTile.vertices[37] = (1.0f / 4.0f) + yOffset;

    // br 2
    t_roadTile.vertices[47] = (1.0f / 4.0f) + xOffset;
    t_roadTile.vertices[48] = yOffset;

    // tr
    t_roadTile.vertices[58] = (1.0f / 4.0f) + xOffset;
    t_roadTile.vertices[59] = (1.0f / 4.0f) + yOffset;
}

bool sg::map::RoadsLayer::CheckTerrainForRoad(const sg::map::Tile& t_tile)
{
    for (auto idx : Tile::Y_INDEX)
    {
        if (t_tile.vertices[idx] < 0.0f)
        {
            return false;
        }
    }

    if (t_tile.vertices[Tile::TL_1_Y] > t_tile.vertices[Tile::BL_1_Y] &&
        t_tile.vertices[Tile::TR_2_Y] <= t_tile.vertices[Tile::BR_1_Y])
    {
        return false;
    }

    if (t_tile.vertices[Tile::TL_1_Y] <= t_tile.vertices[Tile::BL_1_Y] &&
        t_tile.vertices[Tile::TR_2_Y] > t_tile.vertices[Tile::BR_1_Y])
    {
        return false;
    }

    if (t_tile.vertices[Tile::BL_1_Y] > t_tile.vertices[Tile::TL_1_Y] &&
        t_tile.vertices[Tile::BR_1_Y] <= t_tile.vertices[Tile::TR_2_Y])
    {
        return false;
    }

    if (t_tile.vertices[Tile::BL_1_Y] <= t_tile.vertices[Tile::TL_1_Y] &&
        t_tile.vertices[Tile::BR_1_Y] > t_tile.vertices[Tile::TR_2_Y])
    {
        return false;
    }

    return true;
}
