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

#include <utility>
#include "Layer.h"
#include "Log.h"
#include "Tile.h"
#include "gui/MapEditGui.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Layer::Layer(std::shared_ptr<ogl::Window> t_window, const int t_tileCount)
    : window{ std::move(t_window) }
    , tileCount{ t_tileCount }
{
    Log::SG_LOG_DEBUG("[Layer::Layer()] Create Layer.");
}

sg::map::Layer::Layer(std::shared_ptr<ogl::Window> t_window, std::vector<std::shared_ptr<Tile>> t_tiles)
    : window{ std::move(t_window) }
    , tiles{ std::move(t_tiles) }
{
    Log::SG_LOG_DEBUG("[Layer::Layer()] Create Layer.");
}

sg::map::Layer::~Layer() noexcept
{
    Log::SG_LOG_DEBUG("[Layer::~Layer()] Destruct Layer.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::Layer::UpdateTile(const gui::Action t_action, Tile& t_tile)
{
    Log::SG_LOG_DEBUG("[Layer::UpdateTile()] Update tile.");

    // handle change TileType

    auto setTileType = [&](const Tile::TileType t_tileType) -> void
    {
        if (t_tile.type != t_tileType)
        {
            t_tile.UpdateTileType(t_tileType);
            t_tile.VerticesToGpu(*vao);
            Log::SG_LOG_DEBUG("[Layer::UpdateTile()] Tile Id {} was changed to a new type.", t_tile.mapIndex);
        }
    };

    switch (t_action)
    {
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
        break;
    case gui::Action::CREATE_PLANT:
        setTileType(Tile::TileType::PLANTS);
        break;
    default:
        break;
    }
}
