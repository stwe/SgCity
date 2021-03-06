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

sg::map::Map::Map(const int t_tileCount, std::shared_ptr<ogl::Window> t_window)
    : tileCount{ t_tileCount }
    , window{ std::move(t_window) }
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
    terrainLayer->RenderForMousePicking(*window, t_camera);
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

    terrainLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    m_roadsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    m_buildingsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    m_plantsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, WaterLayer::WATER_HEIGHT));
    t_skybox.Render(*window, t_camera);

    t_camera.position.y += distance;
    t_camera.InvertPitch();

    m_waterLayer->GetWaterFbos().UnbindRenderTarget();

    // refraction - everything below the water
    m_waterLayer->GetWaterFbos().BindRefractionFboAsRenderTarget();
    ogl::OpenGL::Clear();
    terrainLayer->Render(t_camera, glm::vec4(0.0f, -1.0f, 0.0f, -WaterLayer::WATER_HEIGHT));
    m_waterLayer->GetWaterFbos().UnbindRenderTarget();

    ogl::OpenGL::DisableClipping();
}

void sg::map::Map::Render(const ogl::camera::Camera& t_camera) const
{
    terrainLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_roadsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_buildingsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_plantsLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
    m_waterLayer->Render(t_camera, glm::vec4(0.0f, 1.0f, 0.0f, 100000.0f));
}

void sg::map::Map::RenderImGui() const
{
    ImGui::Begin("Map - Map edit - Layers");

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("Map");
    ImGui::PopStyleColor();
    ImGui::Text("Mouse x: %.*f", 0, window->GetMouseX()); // NOLINT(clang-diagnostic-double-promotion)
    ImGui::Text("Mouse y: %.*f", 0, window->GetMouseY()); // NOLINT(clang-diagnostic-double-promotion)

    terrainLayer->RenderImGui();
    //m_roadsLayer->RenderImGui();
    m_buildingsLayer->RenderImGui();
    m_plantsLayer->RenderImGui();
    //m_waterLayer->RenderImGui();

    ImGui::End();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Map::Init()
{
    Log::SG_LOG_DEBUG("[Map::Init()] Initialize the map.");

    InitEventDispatcher();

    m_waterLayer = std::make_unique<WaterLayer>(tileCount, window);
    terrainLayer = std::make_unique<TerrainLayer>(tileCount, window);
    m_roadsLayer = std::make_unique<RoadsLayer>(tileCount, window, terrainLayer->tiles);
    m_buildingsLayer = std::make_unique<BuildingsLayer>(window, terrainLayer->tiles);
    m_plantsLayer = std::make_unique<PlantsLayer>(window, terrainLayer->tiles);

    Log::SG_LOG_DEBUG("[Map::Init()] The map was successfully initialized.");
}

void sg::map::Map::InitEventDispatcher() const
{
    Log::SG_LOG_DEBUG("[Map::InitEventDispatcher()] Append listeners.");

    // left mouse button pressed
    event::EventManager::eventDispatcher.appendListener(
        event::SgEventType::MOUSE_BUTTON_PRESSED,
        eventpp::argumentAdapter<void(const event::MouseButtonPressedEvent&)>
        (
            [this](const event::MouseButtonPressedEvent& t_event)
            {
                if (t_event.button == GLFW_MOUSE_BUTTON_LEFT)
                {
                    if (ImGui::GetIO().WantCaptureMouse)
                    {
                        return;
                    }

                    //m_waterLayer->OnLeftMouseButtonPressed();
                    terrainLayer->OnLeftMouseButtonPressed();
                    //m_roadsLayer->OnLeftMouseButtonPressed();
                    //m_buildingsLayer->OnLeftMouseButtonPressed();
                    //m_plantsLayer->OnLeftMouseButtonPressed();
                }
            }
        )
    );

    // left mouse button released
    event::EventManager::eventDispatcher.appendListener(
        event::SgEventType::MOUSE_BUTTON_RELEASED,
        eventpp::argumentAdapter<void(const event::MouseButtonReleasedEvent&)>
        (
            [this](const event::MouseButtonReleasedEvent& t_event)
            {
                if (t_event.button == GLFW_MOUSE_BUTTON_LEFT)
                {
                    //m_waterLayer->OnLeftMouseButtonReleased();
                    terrainLayer->OnLeftMouseButtonReleased();
                    //m_roadsLayer->OnLeftMouseButtonReleased();
                    //m_buildingsLayer->OnLeftMouseButtonReleased();
                    //m_plantsLayer->OnLeftMouseButtonReleased();
                }
            }
        )
    );

    // mouse moved
    event::EventManager::eventDispatcher.appendListener(
        event::SgEventType::MOUSE_MOVED,
        eventpp::argumentAdapter<void(const event::MouseMovedEvent&)>
        (
            [this](const event::MouseMovedEvent& t_event)
            {
                //m_waterLayer->OnMouseMoved();
                terrainLayer->OnMouseMoved();
                //m_roadsLayer->OnMouseMoved();
                //m_buildingsLayer->OnMouseMoved();
                //m_plantsLayer->OnMouseMoved();
            }
        )
    );
}
