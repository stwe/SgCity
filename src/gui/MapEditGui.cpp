#include <imgui.h>
#include "MapEditGui.h"
#include "Log.h"
#include "ogl/input/MouseInput.h"
#include "map/TerrainLayer.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::gui::MapEditGui::Render(const map::Map& t_map)
{
    ImGui::Begin("Game Debug");
    ImGui::Text("Press the Escape key to exit.");
    ImGui::Separator();
    ImGui::Text("Mouse x: %d", ogl::input::MouseInput::GetInstance().GetX());
    ImGui::Text("Mouse y: %d", ogl::input::MouseInput::GetInstance().GetY());

    ImGui::Separator();

    for (auto i{ 0 }; i < 4; ++i)
    {
        if (m_buttons[i])
        {
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.6f, 0.6f)));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.7f, 0.7f)));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.8f, 0.8f)));
            ImGui::Button(m_buttonNames[i].c_str());
            if (ImGui::IsItemClicked(0))
            {
                m_buttons[i] = !m_buttons[i];
            }

            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
        else
        {
            if (ImGui::Button(m_buttonNames[i].c_str()))
            {
                std::fill(m_buttons.begin(), m_buttons.end(), false);
                m_buttons[i] = true;
                action = static_cast<Action>(i);
                Log::SG_LOG_DEBUG("[MapEditGui::Render()] Action: {}", m_buttonNames[i]);
            }
        }
    }

    ImGui::Separator();

    if (t_map.GetTerrainLayer().currentTileIndex != map::TerrainLayer::INVALID_TILE_INDEX && action == Action::INFO)
    {
        const auto& tile{ *t_map.GetTerrainLayer().tiles[t_map.GetTerrainLayer().currentTileIndex] };
        TileInfo(tile);
    }

    ImGui::End();
}

void sg::gui::MapEditGui::TileInfo(const sg::map::Tile& t_tile)
{
    switch (t_tile.type)
    {
        case map::Tile::TileType::NONE : ImGui::Text("Type: Terrain"); break;
        case map::Tile::TileType::RESIDENTIAL : ImGui::Text("Type: Residential"); break;
        case map::Tile::TileType::COMMERCIAL : ImGui::Text("Type: Commercial"); break;
        case map::Tile::TileType::INDUSTRIAL : ImGui::Text("Type: Industrial"); break;
        case map::Tile::TileType::TRAFFIC : ImGui::Text("Type: Road"); break;
    }

    ImGui::Text("Tile index: %d", t_tile.mapIndex);
    ImGui::Text("Tile map x: %f", t_tile.mapX);
    ImGui::Text("Tile map y: %f", t_tile.mapZ);
}
