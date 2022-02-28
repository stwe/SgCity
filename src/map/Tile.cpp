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

#include <glm/geometric.hpp>
#include <imgui.h>
#include "Tile.h"
#include "ogl/buffer/Vbo.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Tile::Tile()
{
    // ...
}

sg::map::Tile::Tile(const float t_mapX, const float t_mapZ, const int t_mapIndex)
    : mapX{ t_mapX }
    , mapZ{ t_mapZ }
    , mapIndex{ t_mapIndex }
{
    Init();
}

sg::map::Tile::~Tile() noexcept
{
    // ...
}

//-------------------------------------------------
// Raise / lower tile vertices
//-------------------------------------------------

void sg::map::Tile::Raise()
{
    vertices[Tile::TL_1_Y] += RAISE_Y;
    vertices[Tile::BL_1_Y] += RAISE_Y;
    vertices[Tile::BR_1_Y] += RAISE_Y;

    vertices[Tile::TL_2_Y] += RAISE_Y;
    vertices[Tile::BR_2_Y] += RAISE_Y;
    vertices[Tile::TR_2_Y] += RAISE_Y;
}

void sg::map::Tile::Lower()
{
    vertices[Tile::TL_1_Y] -= RAISE_Y;
    vertices[Tile::BL_1_Y] -= RAISE_Y;
    vertices[Tile::BR_1_Y] -= RAISE_Y;

    vertices[Tile::TL_2_Y] -= RAISE_Y;
    vertices[Tile::BR_2_Y] -= RAISE_Y;
    vertices[Tile::TR_2_Y] -= RAISE_Y;
}

//-------------------------------------------------
// Normal
//-------------------------------------------------

void sg::map::Tile::UpdateNormal()
{
    const auto normal{ CalcNormal() };

    vertices[Tile::TL_1_N_START_INDEX] = normal.x;
    vertices[Tile::TL_1_N_START_INDEX + 1] = normal.y;
    vertices[Tile::TL_1_N_START_INDEX + 2] = normal.z;

    vertices[Tile::BL_1_N_START_INDEX] = normal.x;
    vertices[Tile::BL_1_N_START_INDEX + 1] = normal.y;
    vertices[Tile::BL_1_N_START_INDEX + 2] = normal.z;

    vertices[Tile::BR_1_N_START_INDEX] = normal.x;
    vertices[Tile::BR_1_N_START_INDEX + 1] = normal.y;
    vertices[Tile::BR_1_N_START_INDEX + 2] = normal.z;

    vertices[Tile::TL_2_N_START_INDEX] = normal.x;
    vertices[Tile::TL_2_N_START_INDEX + 1] = normal.y;
    vertices[Tile::TL_2_N_START_INDEX + 2] = normal.z;

    vertices[Tile::BR_2_N_START_INDEX] = normal.x;
    vertices[Tile::BR_2_N_START_INDEX + 1] = normal.y;
    vertices[Tile::BR_2_N_START_INDEX + 2] = normal.z;

    vertices[Tile::TR_2_N_START_INDEX] = normal.x;
    vertices[Tile::TR_2_N_START_INDEX + 1] = normal.y;
    vertices[Tile::TR_2_N_START_INDEX + 2] = normal.z;
}

//-------------------------------------------------
// Gpu
//-------------------------------------------------

void sg::map::Tile::VerticesToGpu(const ogl::buffer::Vao& t_vao) const
{
    t_vao.vbo->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, mapIndex * static_cast<int64_t>(Tile::BYTES_PER_TILE), Tile::BYTES_PER_TILE, vertices.data());
    ogl::buffer::Vbo::Unbind();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::Tile::RenderImGui() const
{
    ImGui::Begin("Tile info");

    ImGui::Separator();

    switch (type)
    {
        case Tile::TileType::NONE : ImGui::Text("Type: Terrain"); break;
        case Tile::TileType::RESIDENTIAL : ImGui::Text("Type: Residential"); break;
        case Tile::TileType::COMMERCIAL : ImGui::Text("Type: Commercial"); break;
        case Tile::TileType::INDUSTRIAL : ImGui::Text("Type: Industrial"); break;
        case Tile::TileType::TRAFFIC : ImGui::Text("Type: Road"); break;
    }

    ImGui::Text("Tile index: %d", mapIndex);
    ImGui::Text("Tile map x: %f", mapX);
    ImGui::Text("Tile map y: %f", mapZ);

    ImGui::End();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::Tile::Init()
{
    const glm::vec3 tl{ mapX, DEFAULT_HEIGHT, mapZ };
    const glm::vec3 bl{ mapX, DEFAULT_HEIGHT, mapZ + 1.0f };
    const glm::vec3 br{ mapX + 1.0f, DEFAULT_HEIGHT, mapZ + 1.0f };
    const glm::vec3 tr{ mapX + 1.0f, DEFAULT_HEIGHT, mapZ };

    CreateMapIndexColor();

    vertices =
    {
        // pos            // uv       // id color                      // normal
        tl.x, tl.y, tl.z, 0.0f, 1.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,
        bl.x, bl.y, bl.z, 0.0f, 0.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,
        br.x, br.y, br.z, 1.0f, 0.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,

        tl.x, tl.y, tl.z, 0.0f, 1.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,
        br.x, br.y, br.z, 1.0f, 0.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,
        tr.x, tr.y, tr.z, 1.0f, 1.0f, idColor.x, idColor.y, idColor.z, 0.0f, 1.0f, 0.0f,
    };
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::map::Tile::CreateMapIndexColor()
{
    // convert index into an RGB color
    const int r{ (mapIndex & 0x000000FF) >> 0 };
    const int g{ (mapIndex & 0x0000FF00) >> 8 };
    const int b{ (mapIndex & 0x00FF0000) >> 16 };

    idColor = glm::vec3(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f);
}

glm::vec3 sg::map::Tile::CalcNormal() const
{
    // read out positions
    const auto v0{ glm::vec3(vertices[0], vertices[1], vertices[2]) };
    const auto v1{ glm::vec3(vertices[11], vertices[12], vertices[13]) };
    const auto v2{ glm::vec3(vertices[22], vertices[23], vertices[24]) };
    const auto v3{ glm::vec3(vertices[55], vertices[56], vertices[57]) };

    const std::vector vertex = { v0, v1, v2, v3 };

    // calc normal
    glm::vec3 normal{ 0.0f, 0.0f, 0.0f };
    for (auto i{ 0 }; i < 4; ++i)
    {
        const auto j{ (i + 1) % 4 };
        normal.x += (vertex[i].y - vertex[j].y) * (vertex[i].z + vertex[j].z);
        normal.y += (vertex[i].z - vertex[j].z) * (vertex[i].x + vertex[j].x);
        normal.z += (vertex[i].x - vertex[j].x) * (vertex[i].y + vertex[j].y);
    }

    return glm::normalize(normal);
}
