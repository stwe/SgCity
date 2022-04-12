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
#include "Tile.h"
#include "Log.h"
#include "ogl/Window.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/resource/ResourceManager.h"
#include "ogl/input/PickingTexture.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::TerrainLayer::TerrainLayer(std::shared_ptr<ogl::Window> t_window, const int t_tileCount)
    : Layer(std::move(t_window), t_tileCount)
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

    const auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram("E:/Dev/SgCity/resources/shader/picking") };
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

void sg::map::TerrainLayer::UpdateTile(const gui::Action t_action, Tile& t_tile)
{
    // handle base class actions
    Layer::UpdateTile(t_action, t_tile);

    Log::SG_LOG_DEBUG("[TerrainLayer::UpdateTile()] Update terrain tile.");

    // handle raise and lower terrain

    // raise terrain
    if (t_action == gui::Action::RAISE)
    {
        t_tile.Raise();
        UpdateTileVertices(t_tile);
    }

    // lower terrain
    if (t_action == gui::Action::LOWER)
    {
        t_tile.Lower();
        UpdateTileVertices(t_tile);
    }
}

void sg::map::TerrainLayer::Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane) const
{
    ogl::OpenGL::EnableFaceCulling();

    vao->Bind();

    const auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram("E:/Dev/SgCity/resources/shader/map") };
    shaderProgram.Bind();

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", window->GetProjectionMatrix());
    shaderProgram.SetUniform("plane", t_plane);

    const auto mv{ t_camera.GetViewMatrix() * modelMatrix };
    const auto n{ glm::inverseTranspose(glm::mat3(mv)) };
    shaderProgram.SetUniform("normalMatrix", n);

    const auto& grassTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/grass.png") };
    grassTexture.BindForReading(GL_TEXTURE0);
    shaderProgram.SetUniform("diffuseMap", 0);

    const auto& rTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/r.png", true) };
    rTexture.BindForReading(GL_TEXTURE1);
    shaderProgram.SetUniform("rMap", 1);

    const auto& cTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/c.png", true) };
    cTexture.BindForReading(GL_TEXTURE2);
    shaderProgram.SetUniform("cMap", 2);

    const auto& iTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/i.png", true) };
    iTexture.BindForReading(GL_TEXTURE3);
    shaderProgram.SetUniform("iMap", 3);

    const auto& trafficTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/texture/traffic.jpg") };
    trafficTexture.BindForReading(GL_TEXTURE4);
    shaderProgram.SetUniform("trafficMap", 4);

    vao->DrawPrimitives();

    ogl::resource::ShaderProgram::Unbind();
    vao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
}

void sg::map::TerrainLayer::RenderImGui()
{
    m_mapEditGui.RenderImGui();

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
    currentTileIndex = pickingTexture->ReadMapIndex(
        static_cast<int>(window->GetMouseX()),
        static_cast<int>(window->GetMouseY())
    );

    // check tile index
    if (currentTileIndex < 0 || currentTileIndex > static_cast<int>(tiles.size()) - 1)
    {
        currentTileIndex = INVALID_TILE_INDEX;
    }

    // update current tile if the tile index is valid
    if (currentTileIndex != INVALID_TILE_INDEX)
    {
        // get current tile
        m_currentTile = tiles[currentTileIndex];

        // handle raise && lower
        if (m_mapEditGui.action == gui::Action::RAISE || m_mapEditGui.action == gui::Action::LOWER)
        {
            UpdateTile(m_mapEditGui.action, *m_currentTile);
        }

        // handle select
        if (m_mapEditGui.action == gui::Action::SELECT)
        {
            //Log::SG_LOG_INFO("[TerrainLayer::OnLeftMouseButtonPressed()] Start select.");

            // start select
            m_selectFlag = true;

            // set start tile index
            m_startIndex = currentTileIndex;
        }
    }
}

void sg::map::TerrainLayer::OnLeftMouseButtonReleased()
{
    // handle select
    if (m_mapEditGui.action == gui::Action::SELECT)
    {
        //Log::SG_LOG_INFO("[TerrainLayer::OnLeftMouseButtonReleased()] End select.");

        // end select
        m_selectFlag = false;
    }
}

void sg::map::TerrainLayer::OnMouseMoved()
{
    // handle select
    if (m_mapEditGui.action == gui::Action::SELECT && m_selectFlag)
    {
        //Log::SG_LOG_INFO("[TerrainLayer::OnLeftMouseButtonReleased()] Moved on select status.");

        m_endIndex = pickingTexture->ReadMapIndex(
            static_cast<int>(window->GetMouseX()),
            static_cast<int>(window->GetMouseY())
        );

        auto sx{ static_cast<int>(tiles[m_startIndex]->mapX) };
        auto sz{ static_cast<int>(tiles[m_startIndex]->mapZ) };

        auto ex{ static_cast<int>(tiles[m_endIndex]->mapX) };
        auto ez{ static_cast<int>(tiles[m_endIndex]->mapZ) };

        Log::SG_LOG_INFO("Start tile x: {}, y: {}", sx, sz);
        Log::SG_LOG_INFO("End tile x: {}, y: {}", ex, ez);

        if (ez < sz)
        {
            std::swap(sz, ez);
        }

        if (ex < sx)
        {
            std::swap(sx, ex);
        }

        for (auto z{ sz }; z <= ez; ++z)
        {
            for (auto x{ sx }; x <= ex; ++x)
            {
                // todo
                const auto i{ z * tileCount + x };
                UpdateTile(gui::Action::MAKE_INDUSTRIAL_ZONE, *tiles[i]);
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
    for (auto z{ 0 }; z < tileCount; ++z)
    {
        for (auto x{ 0 }; x < tileCount; ++x)
        {
            const auto i{ z * tileCount + x };
            // todo: the normal of each Tile is vec3(0, 1, 0) by default
            tiles.emplace_back(std::make_unique<Tile>(static_cast<float>(x), static_cast<float>(z), i));
        }
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::map::TerrainLayer::AddTileNeighbors() const
{
    for (auto z{ 0 }; z < tileCount; ++z)
    {
        for (auto x{ 0 }; x < tileCount; ++x)
        {
            const auto i{ z * tileCount + x };

            // regular grid
            if (z > 0)
            {
                tiles[i]->n = tiles[(z - 1) * tileCount + x];
            }

            if (z < tileCount - 1)
            {
                tiles[i]->s = tiles[(z + 1) * tileCount + x];
            }

            if (x > 0)
            {
                tiles[i]->w = tiles[z * tileCount + (x - 1)];
            }

            if (x < tileCount - 1)
            {
                tiles[i]->e = tiles[z * tileCount + (x + 1)];
            }

            // connect diagonally
            if (z > 0 && x < tileCount - 1)
            {
                tiles[i]->ne = tiles[(z - 1) * tileCount + (x + 1)];
            }

            if (z > 0 && x > 0)
            {
                tiles[i]->nw = tiles[(z - 1) * tileCount + (x - 1)];
            }

            if (z < tileCount - 1 && x > 0)
            {
                tiles[i]->sw = tiles[(z + 1) * tileCount + (x - 1)];
            }

            if (z < tileCount - 1 && x < tileCount - 1)
            {
                tiles[i]->se = tiles[(z + 1) * tileCount + (x + 1)];
            }
        }
    }
}

void sg::map::TerrainLayer::TilesToGpu()
{
    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateEmptyDynamicVbo(tileCount * tileCount * Tile::BYTES_PER_TILE, tileCount * tileCount * Tile::VERTICES_PER_TILE);

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

        vertices[Tile::BL_1_Y] += t_tile.vertices[Tile::TL_1_Y] - vertices[Tile::BL_1_Y];
        vertices[Tile::BR_1_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::BR_1_Y];

        vertices[Tile::BR_2_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::BR_2_Y];

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

        vertices[Tile::TL_1_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::TL_1_Y];

        vertices[Tile::TL_2_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::TL_2_Y];
        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::BR_2_Y] - vertices[Tile::TR_2_Y];

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

        vertices[Tile::BR_1_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::BR_1_Y];

        vertices[Tile::BR_2_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::BR_2_Y];
        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::TL_2_Y] - vertices[Tile::TR_2_Y];

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

        vertices[Tile::TL_1_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::TL_1_Y];
        vertices[Tile::BL_1_Y] += t_tile.vertices[Tile::BR_1_Y] - vertices[Tile::BL_1_Y];

        vertices[Tile::TL_2_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::TL_2_Y];

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

        vertices[Tile::BR_1_Y] += t_tile.vertices[Tile::TL_1_Y] - vertices[Tile::BR_1_Y];
        vertices[Tile::BR_2_Y] += t_tile.vertices[Tile::TL_2_Y] - vertices[Tile::BR_2_Y];

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

        vertices[Tile::BL_1_Y] += t_tile.vertices[Tile::TR_2_Y] - vertices[Tile::BL_1_Y];

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

        vertices[Tile::TR_2_Y] += t_tile.vertices[Tile::BL_1_Y] - vertices[Tile::TR_2_Y];

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

        vertices[Tile::TL_1_Y] += t_tile.vertices[Tile::BR_1_Y] - vertices[Tile::TL_1_Y];
        vertices[Tile::TL_2_Y] += t_tile.vertices[Tile::BR_2_Y] - vertices[Tile::TL_2_Y];

        tile->UpdateNormal();
        tile->VerticesToGpu(*vao);
    }
}
