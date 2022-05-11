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

#include "TileFactory.h"
#include "Map.h"

std::unique_ptr<sg::map::Tile> sg::map::TileFactory::CreateTile(
    const int t_tileCount,
    const float t_mapX, const float t_mapZ,
    const Tile::TileType t_tileType
)
{
    auto tile{ std::make_unique<Tile>(
        t_mapX, t_mapZ, GetMapIndexFromPosition(t_tileCount, t_mapX, t_mapZ))
    };

    switch (t_tileType)
    {
    case Tile::TileType::NONE:
        tile->type = Tile::TileType::NONE;
        tile->curResidentsOrEmployees = 0;
        tile->maxResidentsOrEmployees = 0;
        return tile;
    case Tile::TileType::RESIDENTIAL:
        tile->type = Tile::TileType::RESIDENTIAL;
        tile->curResidentsOrEmployees = 0;
        tile->maxResidentsOrEmployees = MAX_RESIDENTS_OR_EMPLOYEES;
        return tile;
    case Tile::TileType::COMMERCIAL:
        tile->type = Tile::TileType::COMMERCIAL;
        tile->curResidentsOrEmployees = 0;
        tile->maxResidentsOrEmployees = MAX_RESIDENTS_OR_EMPLOYEES;
        return tile;
    case Tile::TileType::INDUSTRIAL:
        tile->type = Tile::TileType::INDUSTRIAL;
        tile->curResidentsOrEmployees = 0;
        tile->maxResidentsOrEmployees = MAX_RESIDENTS_OR_EMPLOYEES;
        return tile;
    case Tile::TileType::TRAFFIC:
        tile->type = Tile::TileType::TRAFFIC;
        tile->curResidentsOrEmployees = 0;
        tile->maxResidentsOrEmployees = 0;
        return tile;
    case Tile::TileType::PLANTS:
        tile->type = Tile::TileType::PLANTS;
        tile->curResidentsOrEmployees = 0;
        tile->maxResidentsOrEmployees = 0;
        return tile;
    }

    return nullptr;
}

//-------------------------------------------------
// Util
//-------------------------------------------------

int sg::map::TileFactory::GetMapIndexFromPosition(const int t_tileCount, const int t_mapX, const int t_mapZ)
{
    return t_mapZ * t_tileCount + t_mapX;
}

int sg::map::TileFactory::GetMapIndexFromPosition(const int t_tileCount, const float t_mapX, const float t_mapZ)
{
    return GetMapIndexFromPosition(t_tileCount, static_cast<int>(t_mapX), static_cast<int>(t_mapZ));
}
