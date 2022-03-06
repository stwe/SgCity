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
#include "ogl/buffer/Vao.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Layer::Layer()
{
    Log::SG_LOG_DEBUG("[Layer::Layer()] Create Layer.");
}

sg::map::Layer::Layer(const int t_tileCount)
    : tileCount{ t_tileCount }
{
    Log::SG_LOG_DEBUG("[Layer::Layer()] Create Layer.");
}

sg::map::Layer::Layer(const int t_tileCount, std::vector<std::shared_ptr<Tile>> t_tiles)
    : tileCount{ t_tileCount }
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

    // make residential zone (change TileType to RESIDENTIAL)
    if (t_action == gui::Action::MAKE_RESIDENTIAL_ZONE)
    {
        if (t_tile.type != Tile::TileType::RESIDENTIAL)
        {
            t_tile.type = Tile::TileType::RESIDENTIAL;
            Log::SG_LOG_DEBUG("[Layer::UpdateTile()] Tile Id {} was changed to type RESIDENTIAL.", t_tile.mapIndex);
        }
        else
        {
            Log::SG_LOG_DEBUG("[Layer::UpdateTile()] Tile Id {} is already type RESIDENTIAL.", t_tile.mapIndex);
        }
    }

    // make traffic zone (change TileType to TRAFFIC)
    if (t_action == gui::Action::MAKE_TRAFFIC_ZONE)
    {
        if (t_tile.type != Tile::TileType::TRAFFIC)
        {
            t_tile.type = Tile::TileType::TRAFFIC;
            Log::SG_LOG_DEBUG("[Layer::UpdateTile()] Tile Id {} was changed to type TRAFFIC.", t_tile.mapIndex);
        }
        else
        {
            Log::SG_LOG_DEBUG("[Layer::UpdateTile()] Tile Id {} is already type TRAFFIC.", t_tile.mapIndex);
        }
    }
}
