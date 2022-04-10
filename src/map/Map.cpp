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
#include "event/EventManager.h"
#include "eventpp/utilities/argumentadapter.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Map::Map(std::shared_ptr<ogl::Window> t_window, const int t_tileCount)
    : window{ std::move(t_window) }
    , m_tileCount{ t_tileCount }
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

void sg::map::Map::Update()
{
    m_waterLayer->Update();
}

void sg::map::Map::RenderForMousePicking(const ogl::camera::Camera& t_camera) const
{
    m_terrainLayer->RenderForMousePicking(*window, t_camera);
}

void sg::map::Map::RenderForWater(ogl::camera::Camera& t_camera, const ogl::resource::Skybox& t_skybox) const
{
    ogl::OpenGL::EnableClipping();
    ogl::OpenGL::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // reflection - everything above the water
    m_waterLayer->GetWaterFbos().BindReflectionFboAsRenderTarget();
    ogl::OpenGL::Clear();

    const auto distance{ 2.0f * (t_camera.position.y - WaterLayer::WATER_HEIGHT) };
    t_camera.position.y -= distance;
    t_camera.InvertPitch();

    m_terrainLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    m_roadsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    m_buildingsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    m_plantsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    //t_skybox.Render(t_window, t_camera);

    t_camera.position.y += distance;
    t_camera.InvertPitch();

    m_waterLayer->GetWaterFbos().UnbindRenderTarget();

    // refraction - everything below the water
    m_waterLayer->GetWaterFbos().BindRefractionFboAsRenderTarget();
    ogl::OpenGL::Clear();
    m_terrainLayer->Render(t_camera, glm::vec4(0.0f, -1.0f, 0.0f, -WaterLayer::WATER_HEIGHT));
    m_waterLayer->GetWaterFbos().UnbindRenderTarget();

    ogl::OpenGL::DisableClipping();
}

void sg::map::Map::Render(const ogl::camera::Camera& t_camera) const
{
    m_terrainLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_roadsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_buildingsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_plantsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_waterLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
}

void sg::map::Map::RenderImGui() const
{
    ImGui::Begin("Map");
    ImGui::Text("Mouse x: %.*f", 0, window->GetMouseX());
    ImGui::Text("Mouse y: %.*f", 0, window->GetMouseY());
    ImGui::End();

    m_terrainLayer->RenderImGui();
    m_roadsLayer->RenderImGui();
    m_buildingsLayer->RenderImGui();
    m_plantsLayer->RenderImGui();
    m_waterLayer->RenderImGui();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Map::Init()
{
    Log::SG_LOG_DEBUG("[Map::Init()] Initialize the map.");

    InitEventDispatcher();

    m_waterLayer = std::make_unique<WaterLayer>(window, m_tileCount);
    m_terrainLayer = std::make_unique<TerrainLayer>(window, m_tileCount);
    m_roadsLayer = std::make_unique<RoadsLayer>(window, m_terrainLayer->tiles);
    m_buildingsLayer = std::make_unique<BuildingsLayer>(window, m_terrainLayer->tiles);
    m_plantsLayer = std::make_unique<PlantsLayer>(window, m_terrainLayer->tiles);

    Log::SG_LOG_DEBUG("[Map::Init()] The map was successfully initialized.");
}

void sg::map::Map::InitEventDispatcher() const
{
    Log::SG_LOG_DEBUG("[Map::InitEventDispatcher()] Append listeners.");

    // left mouse button pressed
    event::EventManager::eventDispatcher.appendListener(
        event::SgEventType::MOUSE_BUTTON_PRESSED,
        eventpp::argumentAdapter<void(const event::MouseButtonPressedEvent&)>(
            [this](const event::MouseButtonPressedEvent& t_event)
            {
                if (t_event.button == GLFW_MOUSE_BUTTON_LEFT)
                {
                    // todo: test
                    if (ImGui::GetIO().WantCaptureMouse)
                    {
                        return;
                    }

                    //m_waterLayer->OnLeftMouseButtonPressed();
                    m_terrainLayer->OnLeftMouseButtonPressed();
                    //m_roadsLayer->OnLeftMouseButtonPressed();
                    //m_buildingsLayer->OnLeftMouseButtonPressed();
                    //m_plantsLayer->OnLeftMouseButtonPressed();
                }
            }
        )
    );
}
