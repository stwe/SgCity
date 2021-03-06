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
#include "TerrainLayer.h"
#include "TileFactory.h"
#include "Game.h"
#include "Log.h"
#include "Map.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/resource/ResourceManager.h"
#include "ogl/input/PickingTexture.h"
#include "event/EventManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::TerrainLayer::TerrainLayer(const int t_tileCount, std::shared_ptr<ogl::Window> t_window)
    : Layer(std::move(t_window))
    , m_tileCount{ t_tileCount }
{
    Log::SG_LOG_DEBUG("[TerrainLayer::TerrainLayer()] Create TerrainLayer.");

    Init();
}

sg::map::TerrainLayer::~TerrainLayer() noexcept
{
    Log::SG_LOG_DEBUG("[TerrainLayer::~TerrainLayer()] Destruct TerrainLayer.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::TerrainLayer::RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera)
{
    if (!pickingTexture)
    {
        pickingTexture = std::make_unique<ogl::input::PickingTexture>(t_window.GetWidth(), t_window.GetHeight());
    }

    pickingTexture->EnableWriting();

    ogl::OpenGL::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    ogl::OpenGL::Clear();

    vao->Bind();

    const auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram(Game::RESOURCES_PATH + "shader/picking") };
    shaderProgram.Bind();

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", t_window.GetProjectionMatrix());

    vao->DrawPrimitives();

    ogl::resource::ShaderProgram::Unbind();
    vao->Unbind();

    pickingTexture->DisableWriting();
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::map::TerrainLayer::Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane)
{
    ogl::OpenGL::EnableFaceCulling();

    vao->Bind();

    const auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram(Game::RESOURCES_PATH + "shader/layer/terrain") };
    shaderProgram.Bind();

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", window->GetProjectionMatrix());
    shaderProgram.SetUniform("plane", t_plane);

    const auto mv{ t_camera.GetViewMatrix() * modelMatrix };
    const auto n{ glm::inverseTranspose(glm::mat3(mv)) };
    shaderProgram.SetUniform("normalMatrix", n);

    const auto& grassTexture{ ogl::resource::ResourceManager::LoadTexture(Game::RESOURCES_PATH + "texture/grass.png") };
    grassTexture.BindForReading(GL_TEXTURE0);
    shaderProgram.SetUniform("diffuseMap", 0);

    const auto& rTexture{ ogl::resource::ResourceManager::LoadTexture(Game::RESOURCES_PATH + "texture/r.png", true) };
    rTexture.BindForReading(GL_TEXTURE1);
    shaderProgram.SetUniform("rMap", 1);

    const auto& cTexture{ ogl::resource::ResourceManager::LoadTexture(Game::RESOURCES_PATH + "texture/c.png", true) };
    cTexture.BindForReading(GL_TEXTURE2);
    shaderProgram.SetUniform("cMap", 2);

    const auto& iTexture{ ogl::resource::ResourceManager::LoadTexture(Game::RESOURCES_PATH + "texture/i.png", true) };
    iTexture.BindForReading(GL_TEXTURE3);
    shaderProgram.SetUniform("iMap", 3);

    const auto& trafficTexture{ ogl::resource::ResourceManager::LoadTexture(Game::RESOURCES_PATH + "texture/t.png") };
    trafficTexture.BindForReading(GL_TEXTURE4);
    shaderProgram.SetUniform("tMap", 4);

    vao->DrawPrimitives();

    ogl::resource::ShaderProgram::Unbind();
    vao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
}

void sg::map::TerrainLayer::RenderImGui()
{
    m_mapEditGui.RenderImGui();

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("Terrain Layer");
    ImGui::PopStyleColor();

    ImGui::Text("Regions: %d", m_numRegions);

    if (m_currentTile)
    {
        m_currentTile->RenderImGui();
    }
}

//-------------------------------------------------
// Listeners
//-------------------------------------------------

void sg::map::TerrainLayer::OnLeftMouseButtonPressed()
{
    // read tile index under mouse
    currentTileIndex = ReadTileIndexUnderMouse();

    // handle actions only if the tile index is valid
    if (currentTileIndex != INVALID_TILE_INDEX)
    {
        if (m_mapEditGui.action != gui::Action::INFO)
        {
            m_selectFlag = true;
        }
        else
        {
            m_currentTile = tiles[currentTileIndex];
        }
    }
}

void sg::map::TerrainLayer::OnLeftMouseButtonReleased()
{
    // handle select
    if (m_selectFlag)
    {
        // reset select state
        m_selectFlag = false;

        // are several tiles selected?
        if (!m_selectedIndices.empty())
        {
            // for each selected tile ...
            for (const auto i : m_selectedIndices)
            {
                // mark tile as unselected
                SetTileSelectedState(false, *tiles[i]);

                // change tile by current menu action
                ChangeTileByAction(m_mapEditGui.action, *tiles[i]);
            }

            // reset array
            m_selectedIndices.clear();
        }
        else
        {
            // handle single click on a tile
            ChangeTileByAction(m_mapEditGui.action, *tiles[currentTileIndex]);
        }
    }
}

void sg::map::TerrainLayer::OnMouseMoved()
{
    // handle select
    if (m_selectFlag)
    {
        // read tile index under mouse
        m_currentLastIndex = ReadTileIndexUnderMouse();

        // handle select only if the tile indices are valid
        if (currentTileIndex != INVALID_TILE_INDEX &&
            m_currentLastIndex != INVALID_TILE_INDEX &&
            m_lastIndex != m_currentLastIndex)
        {
            // mark the previously selected tiles as unselected
            if (!m_selectedIndices.empty())
            {
                for (const auto i : m_selectedIndices)
                {
                    // mark tile as unselected
                    SetTileSelectedState(false, *tiles[i]);
                }

                // reset array
                m_selectedIndices.clear();
            }

            auto sx{ static_cast<int>(tiles[currentTileIndex]->mapX) };
            auto sz{ static_cast<int>(tiles[currentTileIndex]->mapZ) };

            auto ex{ static_cast<int>(tiles[m_currentLastIndex]->mapX) };
            auto ez{ static_cast<int>(tiles[m_currentLastIndex]->mapZ) };

            if (ez < sz)
            {
                std::swap(sz, ez);
            }

            if (ex < sx)
            {
                std::swap(sx, ex);
            }

            // mark as selected
            for (auto z{ sz }; z <= ez; ++z)
            {
                for (auto x{ sx }; x <= ex; ++x)
                {
                    // calc tile map index
                    const auto i{ TileFactory::GetMapIndexFromPosition(m_tileCount, x, z) };

                    // only tiles of type NONE can be selected
                    if (tiles[i]->type == Tile::TileType::NONE)
                    {
                        // store tile map index
                        m_selectedIndices.push_back(i);

                        // mark as selected
                        SetTileSelectedState(true, *tiles[i]);

                        m_lastIndex = m_currentLastIndex;
                    }
                }
            }
        }
    }
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::TerrainLayer::Init()
{
    Log::SG_LOG_DEBUG("[TerrainLayer::Init()] Initialize the TerrainLayer.");

    position = glm::vec3(0.0f);

    modelMatrix = ogl::math::Transform::CreateModelMatrix(
        position,
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    );

    CreateTiles();
    AddTileNeighbors();
    TilesToGpu();

    Log::SG_LOG_DEBUG("[TerrainLayer::Init()] The TerrainLayer was successfully initialized.");
}

void sg::map::TerrainLayer::CreateTiles()
{
    for (auto z{ 0 }; z < m_tileCount; ++z)
    {
        for (auto x{ 0 }; x < m_tileCount; ++x)
        {
            tiles.emplace_back(TileFactory::CreateTile(m_tileCount, static_cast<float>(x), static_cast<float>(z), Tile::TileType::NONE));
        }
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::map::TerrainLayer::AddTileNeighbors() const
{
    for (auto z{ 0 }; z < m_tileCount; ++z)
    {
        for (auto x{ 0 }; x < m_tileCount; ++x)
        {
            const auto i{ z * m_tileCount + x };

            // regular grid
            if (z > 0)
            {
                tiles[i]->n = tiles[(z - 1) * m_tileCount + x];
                tiles[i]->neighbors.push_back(tiles[i]->n);
            }

            if (z < m_tileCount - 1)
            {
                tiles[i]->s = tiles[(z + 1) * m_tileCount + x];
                tiles[i]->neighbors.push_back(tiles[i]->s);
            }

            if (x > 0)
            {
                tiles[i]->w = tiles[z * m_tileCount + (x - 1)];
                tiles[i]->neighbors.push_back(tiles[i]->w);
            }

            if (x < m_tileCount - 1)
            {
                tiles[i]->e = tiles[z * m_tileCount + (x + 1)];
                tiles[i]->neighbors.push_back(tiles[i]->e);
            }

            // connect diagonally
            if (z > 0 && x < m_tileCount - 1)
            {
                tiles[i]->ne = tiles[(z - 1) * m_tileCount + (x + 1)];
                tiles[i]->neighbors.push_back(tiles[i]->ne);
            }

            if (z > 0 && x > 0)
            {
                tiles[i]->nw = tiles[(z - 1) * m_tileCount + (x - 1)];
                tiles[i]->neighbors.push_back(tiles[i]->nw);
            }

            if (z < m_tileCount - 1 && x > 0)
            {
                tiles[i]->sw = tiles[(z + 1) * m_tileCount + (x - 1)];
                tiles[i]->neighbors.push_back(tiles[i]->sw);
            }

            if (z < m_tileCount - 1 && x < m_tileCount - 1)
            {
                tiles[i]->se = tiles[(z + 1) * m_tileCount + (x + 1)];
                tiles[i]->neighbors.push_back(tiles[i]->se);
            }
        }
    }
}

void sg::map::TerrainLayer::TilesToGpu()
{
    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateEmptyDynamicVbo(m_tileCount * m_tileCount * Tile::BYTES_PER_TILE, m_tileCount * m_tileCount * Tile::VERTICES_PER_TILE);

    for (const auto& tile : tiles)
    {
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateTileVertices(Tile& t_tile) const
{
    t_tile.UpdateNormal();
    t_tile.VerticesToGpu(*vao);

    UpdateNorthNeighbor(t_tile);
    UpdateSouthNeighbor(t_tile);
    UpdateEastNeighbor(t_tile);
    UpdateWestNeighbor(t_tile);

    UpdateNorthEastNeighbor(t_tile);
    UpdateNorthWestNeighbor(t_tile);
    UpdateSouthEastNeighbor(t_tile);
    UpdateSouthWestNeighbor(t_tile);
}

void sg::map::TerrainLayer::UpdateNorthNeighbor(const Tile& t_tile) const
{
    if (t_tile.n)
    {
        const auto& tile{ t_tile.n };
        auto& vertices{ tile->vertices };

        vertices[Tile::BL_1_POSITION_Y] += t_tile.vertices[Tile::TL_1_POSITION_Y] - vertices[Tile::BL_1_POSITION_Y];
        vertices[Tile::BR_1_POSITION_Y] += t_tile.vertices[Tile::TR_2_POSITION_Y] - vertices[Tile::BR_1_POSITION_Y];

        vertices[Tile::BR_2_POSITION_Y] += t_tile.vertices[Tile::TR_2_POSITION_Y] - vertices[Tile::BR_2_POSITION_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateSouthNeighbor(const Tile& t_tile) const
{
    if (t_tile.s)
    {
        const auto& tile{ t_tile.s };
        auto& vertices{ tile->vertices };

        vertices[Tile::TL_1_POSITION_Y] += t_tile.vertices[Tile::BL_1_POSITION_Y] - vertices[Tile::TL_1_POSITION_Y];

        vertices[Tile::TL_2_POSITION_Y] += t_tile.vertices[Tile::BL_1_POSITION_Y] - vertices[Tile::TL_2_POSITION_Y];
        vertices[Tile::TR_2_POSITION_Y] += t_tile.vertices[Tile::BR_2_POSITION_Y] - vertices[Tile::TR_2_POSITION_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateWestNeighbor(const Tile& t_tile) const
{
    if (t_tile.w)
    {
        const auto& tile{ t_tile.w };
        auto& vertices{ tile->vertices };

        vertices[Tile::BR_1_POSITION_Y] += t_tile.vertices[Tile::BL_1_POSITION_Y] - vertices[Tile::BR_1_POSITION_Y];

        vertices[Tile::BR_2_POSITION_Y] += t_tile.vertices[Tile::BL_1_POSITION_Y] - vertices[Tile::BR_2_POSITION_Y];
        vertices[Tile::TR_2_POSITION_Y] += t_tile.vertices[Tile::TL_2_POSITION_Y] - vertices[Tile::TR_2_POSITION_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateEastNeighbor(const Tile& t_tile) const
{
    if (t_tile.e)
    {
        const auto& tile{ t_tile.e };
        auto& vertices{ tile->vertices };

        vertices[Tile::TL_1_POSITION_Y] += t_tile.vertices[Tile::TR_2_POSITION_Y] - vertices[Tile::TL_1_POSITION_Y];
        vertices[Tile::BL_1_POSITION_Y] += t_tile.vertices[Tile::BR_1_POSITION_Y] - vertices[Tile::BL_1_POSITION_Y];

        vertices[Tile::TL_2_POSITION_Y] += t_tile.vertices[Tile::TR_2_POSITION_Y] - vertices[Tile::TL_2_POSITION_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateNorthWestNeighbor(const Tile& t_tile) const
{
    if (t_tile.nw)
    {
        const auto& tile{ t_tile.nw };
        auto& vertices{ tile->vertices };

        vertices[Tile::BR_1_POSITION_Y] += t_tile.vertices[Tile::TL_1_POSITION_Y] - vertices[Tile::BR_1_POSITION_Y];
        vertices[Tile::BR_2_POSITION_Y] += t_tile.vertices[Tile::TL_2_POSITION_Y] - vertices[Tile::BR_2_POSITION_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateNorthEastNeighbor(const Tile& t_tile) const
{
    if (t_tile.ne)
    {
        const auto& tile{ t_tile.ne };
        auto& vertices{ tile->vertices };

        vertices[Tile::BL_1_POSITION_Y] += t_tile.vertices[Tile::TR_2_POSITION_Y] - vertices[Tile::BL_1_POSITION_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateSouthWestNeighbor(const Tile& t_tile) const
{
    if (t_tile.sw)
    {
        const auto& tile{ t_tile.sw };
        auto& vertices{ tile->vertices };

        vertices[Tile::TR_2_POSITION_Y] += t_tile.vertices[Tile::BL_1_POSITION_Y] - vertices[Tile::TR_2_POSITION_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

void sg::map::TerrainLayer::UpdateSouthEastNeighbor(const Tile& t_tile) const
{
    if (t_tile.se)
    {
        const auto& tile{ t_tile.se };
        auto& vertices{ tile->vertices };

        vertices[Tile::TL_1_POSITION_Y] += t_tile.vertices[Tile::BR_1_POSITION_Y] - vertices[Tile::TL_1_POSITION_Y];
        vertices[Tile::TL_2_POSITION_Y] += t_tile.vertices[Tile::BR_2_POSITION_Y] - vertices[Tile::TL_2_POSITION_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}

int sg::map::TerrainLayer::ReadTileIndexUnderMouse() const
{
    // read tile index under mouse
    auto index{ pickingTexture->ReadMapIndex(
        static_cast<int>(window->GetMouseX()),
        static_cast<int>(window->GetMouseY())
    ) };

    // check tile index
    if (index < 0 || index > static_cast<int>(tiles.size()) - 1)
    {
        index = INVALID_TILE_INDEX;
    }

    return index;
}

void sg::map::TerrainLayer::SetTileSelectedState(const bool t_selected, Tile& t_tile) const
{
    t_tile.UpdateSelected(t_selected);
    t_tile.VerticesToGpu(*vao);
}

void sg::map::TerrainLayer::ChangeTileByAction(const gui::Action t_action, Tile& t_tile)
{
    // helper
    auto setTileType = [&](const Tile::TileType t_tileType) -> void
    {
        if (t_tile.type != t_tileType)
        {
            t_tile.UpdateTileType(t_tileType);
            t_tile.VerticesToGpu(*vao);
        }
    };

    switch (t_action) // NOLINT(clang-diagnostic-switch-enum)
    {
    case gui::Action::RAISE:
        t_tile.Raise();
        UpdateTileVertices(t_tile);
        break;
    case gui::Action::LOWER:
        t_tile.Lower();
        UpdateTileVertices(t_tile);
        break;
    case gui::Action::MAKE_RESIDENTIAL_ZONE:
        setTileType(Tile::TileType::RESIDENTIAL);
        break;
    case gui::Action::MAKE_COMMERCIAL_ZONE:
        setTileType(Tile::TileType::COMMERCIAL);
        break;
    case gui::Action::MAKE_INDUSTRIAL_ZONE:
        setTileType(Tile::TileType::INDUSTRIAL);
        break;
    case gui::Action::MAKE_TRAFFIC_ZONE:
        setTileType(Tile::TileType::TRAFFIC);

        // notifies all listeners that a road should be created
        // in the RoadsLayer, a listener handle the CREATE_ROAD event
        event::EventManager::eventDispatcher.dispatch(
            event::SgEventType::CREATE_ROAD,
            event::CreateRoadEvent(t_tile.mapIndex)
        );
        break;
    case gui::Action::CREATE_PLANT:
        setTileType(Tile::TileType::PLANTS);
        break;
    default:
        break;
    }

    FindConnectedRegions();
}

//-------------------------------------------------
// Regions
//-------------------------------------------------

void sg::map::TerrainLayer::FindConnectedRegions()
{
    auto regions{ 0 };

    for (const auto& tile : tiles)
    {
        tile->region = Tile::NO_REGION;
    }

    for (const auto& tile : tiles)
    {
        auto found{ false };
        for (const auto tileType : Tile::REGION_TILE_TYPES)
        {
            if (tileType == tile->type)
            {
                found = true;
                break;
            }
        }

        if (tile->region == Tile::NO_REGION && found)
        {
            regions++;
            DepthSearch(*tile, regions);
        }
    }

    m_numRegions = regions;
}

void sg::map::TerrainLayer::DepthSearch(Tile& t_startTile, const int t_region)
{
    if (t_startTile.region != Tile::NO_REGION)
    {
        return;
    }

    auto found{ false };
    for (const auto tileType : Tile::REGION_TILE_TYPES)
    {
        if (tileType == t_startTile.type)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        return;
    }

    t_startTile.region = t_region;

    // todo: create && update regions minimap
    // changing the color needs also a Vbo update
    //t_startTile.SetColor(static_cast<glm::vec3>(m_randomColors[t_region - 1]));
    //UpdateMapVboByTileIndex(t_startTile.GetMapIndex());

    for (const auto& neighbor : t_startTile.neighbors)
    {
            DepthSearch(*neighbor, t_region);
    }
}
