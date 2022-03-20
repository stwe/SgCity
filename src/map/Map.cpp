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

#include "Map.h"
#include "Log.h"
#include "WaterLayer.h"
#include "TerrainLayer.h"
#include "RoadsLayer.h"
#include "BuildingsLayer.h"
#include "PlantsLayer.h"
#include "gui/MapEditGui.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/WaterFbos.h"
#include "ogl/input/PickingTexture.h"
#include "ogl/input/MouseInput.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Map::Map(const int t_tileCount)
    : m_tileCount{ t_tileCount }
{
    Log::SG_LOG_DEBUG("[Map::Map()] Create Map.");

    Init();
}

sg::map::Map::~Map() noexcept
{
    Log::SG_LOG_DEBUG("[Map::~Map()] Destruct Map.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::Map::Input()
{
    UpdateCurrentTileIndex();
}

void sg::map::Map::Update(const gui::Action t_action)
{
    if (currentTileIndex != INVALID_TILE_INDEX)
    {
        m_currentTile = m_terrainLayer->tiles[currentTileIndex];

        m_terrainLayer->UpdateTile(t_action, *m_currentTile);
        currentTileIndex = INVALID_TILE_INDEX;
    }

    m_waterLayer->Update();
}

void sg::map::Map::RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const
{
    m_terrainLayer->RenderForMousePicking(t_window, t_camera);
}

void sg::map::Map::RenderForWater(
    const ogl::Window& t_window,
    ogl::camera::Camera& t_camera,
    const ogl::resource::Skybox& t_skybox
) const
{
    ogl::OpenGL::EnableClipping();
    ogl::OpenGL::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // reflection - everything above the water
    m_waterLayer->GetWaterFbos().BindReflectionFboAsRenderTarget();
    ogl::OpenGL::Clear();

    const auto distance{ 2.0f * (t_camera.GetPosition().y - WaterLayer::WATER_HEIGHT) };
    t_camera.GetPosition().y -= distance;
    t_camera.InvertPitch();
    t_camera.Update();

    m_terrainLayer->Render(t_window, t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    m_roadsLayer->Render(t_window, t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    m_buildingsLayer->Render(t_window, t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    m_plantsLayer->Render(t_window, t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    //t_skybox.Render(t_window, t_camera);

    t_camera.GetPosition().y += distance;
    t_camera.InvertPitch();
    t_camera.Update();

    m_waterLayer->GetWaterFbos().UnbindRenderTarget();

    // refraction - everything below the water
    m_waterLayer->GetWaterFbos().BindRefractionFboAsRenderTarget();
    ogl::OpenGL::Clear();
    m_terrainLayer->Render(t_window, t_camera, glm::vec4(0.0f, -1.0f, 0.0f, -WaterLayer::WATER_HEIGHT));
    m_waterLayer->GetWaterFbos().UnbindRenderTarget();

    ogl::OpenGL::DisableClipping();
}

void sg::map::Map::Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const
{
    m_terrainLayer->Render(t_window, t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_roadsLayer->Render(t_window, t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_buildingsLayer->Render(t_window, t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_plantsLayer->Render(t_window, t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));

    m_waterLayer->Render(t_window, t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
}

void sg::map::Map::RenderImGui() const
{
    ImGui::Begin("Map");

    ImGui::Text("reflection");
    if (m_waterLayer->GetWaterFbos().reflectionColorTextureId)
    {
        ImGui::Image(
            reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(m_waterLayer->GetWaterFbos().reflectionColorTextureId)),
            ImVec2(128.0f, 128.0f),
            ImVec2(0.0f, 0.0f),
            ImVec2(1.0f, -1.0f)
        );
    }

    ImGui::Text("refraction");
    if (m_waterLayer->GetWaterFbos().refractionColorTextureId)
    {
        ImGui::Image(
            reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(m_waterLayer->GetWaterFbos().refractionColorTextureId)),
            ImVec2(128.0f, 128.0f),
            ImVec2(0.0f, 0.0f),
            ImVec2(1.0f, -1.0f)
        );
    }

    ImGui::End();

    if (m_currentTile)
    {
        m_currentTile->RenderImGui();
    }
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Map::Init()
{
    Log::SG_LOG_DEBUG("[Map::Init()] Initialize the map.");

    m_waterLayer = std::make_unique<WaterLayer>(m_tileCount);
    m_terrainLayer = std::make_unique<TerrainLayer>(m_tileCount);
    m_roadsLayer = std::make_unique<RoadsLayer>(m_terrainLayer->tiles);
    m_buildingsLayer = std::make_unique<BuildingsLayer>(m_terrainLayer->tiles);
    m_plantsLayer = std::make_unique<PlantsLayer>(m_terrainLayer->tiles);

    Log::SG_LOG_DEBUG("[Map::Init()] The map was successfully initialized.");
}

//-------------------------------------------------
// Mouse picking
//-------------------------------------------------

void sg::map::Map::UpdateCurrentTileIndex()
{
    currentTileIndex = m_terrainLayer->GetPickingTexture().ReadMapIndex(
        ogl::input::MouseInput::GetInstance().GetX(),
        ogl::input::MouseInput::GetInstance().GetY()
    );

    if (currentTileIndex < 0 || currentTileIndex > static_cast<int>(m_terrainLayer->tiles.size()) - 1)
    {
        currentTileIndex = INVALID_TILE_INDEX;
    }
}
