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

#pragma once

#include "Tile.h"

//-------------------------------------------------
// TileFactory
//-------------------------------------------------

namespace sg::map
{
    /**
     * Factory to create Tile objects.
     */
    class TileFactory
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * The maximum number of residents / employees.
         */
        static constexpr auto MAX_RESIDENTS_OR_EMPLOYEES{ 50 };

        //-------------------------------------------------
        // Create
        //-------------------------------------------------

        /**
         * Constructs new Tile objects and sets some defaults.
         *
         * @param t_tileCount The number of tiles in x and z direction.
         * @param t_mapX The top left x position of the Tile in local space.
         * @param t_mapZ The top left z position of the Tile in local space.
         * @param t_tileType The type of the Tile.
         */
        static std::unique_ptr<Tile> CreateTile(int t_tileCount, float t_mapX, float t_mapZ, Tile::TileType t_tileType);

        //-------------------------------------------------
        // Util
        //-------------------------------------------------

        /**
         * Converts a 2D array index into a 1D index.
         *
         * @param t_tileCount The number of tiles in x and z direction.
         * @param t_mapX The x position.
         * @param t_mapZ The z or y position.
         *
         * @return The 1D index.
         */
        static int GetMapIndexFromPosition(int t_tileCount, int t_mapX, int t_mapZ);

        /**
         * Converts a 2D array index into a 1D index.
         *
         * @param t_tileCount The number of tiles in x and z direction.
         * @param t_mapX The x position.
         * @param t_mapZ The z or y position.
         *
         * @return The 1D index.
         */
        static int GetMapIndexFromPosition(int t_tileCount, float t_mapX, float t_mapZ);

    protected:


    private:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TileFactory() = default;
    };
}
