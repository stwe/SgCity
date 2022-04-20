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

#include <array>
#include <glm/vec3.hpp>
#include "ogl/buffer/Vao.h"

namespace sg::map
{
    /**
     * Represents a Tile Object.
     */
    class Tile
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * Position (3 Floats), Uv (2 Floats), Id color (3 Floats), Normal (3 Floats),
         * TextureNr (1 Float), Selected (1 Float)
         * -> 13 Floats x 4 Bytes per Float -> 52 Bytes per Vertex
         * -> 6 Vertices per Tile = 312 Bytes
         */
        static constexpr auto BYTES_PER_TILE{ 312 };

        /**
         * Vertices per Tile.
         */
        static constexpr auto VERTICES_PER_TILE{ 6 };

        /**
         * The default height of the Tile.
         */
        static constexpr auto DEFAULT_HEIGHT{ 0.0f };

        /**
         * The Tile is increased/lowered by this value.
         */
        static constexpr auto RAISE_Y{ 0.5f };

        /*
            tl       tr
            +--------+
            |  +   2 |
            |    +   |
            | 1    + |
            +--------+
            bl       br

            1) tl, bl, br
            2) tl, br, tr
        */

        // position.y array index of each vertex

        static constexpr auto TL_1_POSITION_Y{ 1 };
        static constexpr auto BL_1_POSITION_Y{ 14 };
        static constexpr auto BR_1_POSITION_Y{ 27 };

        static constexpr auto TL_2_POSITION_Y{ 40 };
        static constexpr auto BR_2_POSITION_Y{ 53 };
        static constexpr auto TR_2_POSITION_Y{ 66 };

        static constexpr std::array<int, 6> Y_INDEX =
        {
            TL_1_POSITION_Y, BL_1_POSITION_Y, BR_1_POSITION_Y,
            TL_2_POSITION_Y, BR_2_POSITION_Y, TR_2_POSITION_Y
        };

        // uv.x array index of each vertex

        static constexpr auto TL_1_UV_X{ 3 };
        static constexpr auto BL_1_UV_X{ 16 };
        static constexpr auto BR_1_UV_X{ 29 };

        static constexpr auto TL_2_UV_X{ 42 };
        static constexpr auto BR_2_UV_X{ 55 };
        static constexpr auto TR_2_UV_X{ 68 };

        // normal.x array index of each vertex

        static constexpr auto TL_1_NORMAL_X{ 8 };
        static constexpr auto BL_1_NORMAL_X{ 21 };
        static constexpr auto BR_1_NORMAL_X{ 34 };

        static constexpr auto TL_2_NORMAL_X{ 47 };
        static constexpr auto BR_2_NORMAL_X{ 60 };
        static constexpr auto TR_2_NORMAL_X{ 73 };

        // texture number array index of each vertex

        static constexpr auto TL_1_TEXTURE_NR{ 11 };
        static constexpr auto BL_1_TEXTURE_NR{ 24 };
        static constexpr auto BR_1_TEXTURE_NR{ 37 };

        static constexpr auto TL_2_TEXTURE_NR{ 50 };
        static constexpr auto BR_2_TEXTURE_NR{ 63 };
        static constexpr auto TR_2_TEXTURE_NR{ 76 };

        // selected state array index of each vertex

        static constexpr auto TL_1_SELECTED{ 12 };
        static constexpr auto BL_1_SELECTED{ 25 };
        static constexpr auto BR_1_SELECTED{ 38 };

        static constexpr auto TL_2_SELECTED{ 51 };
        static constexpr auto BR_2_SELECTED{ 64 };
        static constexpr auto TR_2_SELECTED{ 77 };

        //-------------------------------------------------
        // Types
        //-------------------------------------------------

        enum class TileType
        {
            NONE,
            RESIDENTIAL,
            COMMERCIAL,
            INDUSTRIAL,
            TRAFFIC,
            PLANTS
        };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * Vertices of the Tile.
         */
        std::vector<float> vertices;

        /**
         * The top left x position of the Tile in local space.
         */
        float mapX{ 0.0f };

        /**
         * The top left z position of the Tile in local space.
         */
        float mapZ{ 0.0f };

        /**
         * The index of this Tile in the Map array.
         */
        int mapIndex{ 0 };

        /**
         * A unique color used for mouse picking.
         */
        glm::vec3 idColor{ glm::vec3(0.0f) };

        /**
         * The type of the Tile.
         */
        TileType type{ TileType::NONE };

        /**
         * True or false depending on whether this tile is selected or not.
         */
        bool selected{ false };

        // Neighbors

        std::shared_ptr<Tile> n;
        std::shared_ptr<Tile> s;
        std::shared_ptr<Tile> e;
        std::shared_ptr<Tile> w;

        std::shared_ptr<Tile> nw;
        std::shared_ptr<Tile> ne;
        std::shared_ptr<Tile> sw;
        std::shared_ptr<Tile> se;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Tile();

        /**
         * Constructs a new Tile object.
         *
         * @param t_mapX The top left x position of the Tile in local space.
         * @param t_mapZ The top left z position of the Tile in local space.
         * @param t_mapIndex The index of this Tile in the Map array.
         */
        Tile(float t_mapX, float t_mapZ, int t_mapIndex);

        Tile(const Tile& t_other) = delete;
        Tile(Tile&& t_other) noexcept = delete;
        Tile& operator=(const Tile& t_other) = delete;
        Tile& operator=(Tile&& t_other) noexcept = delete;

        virtual ~Tile() noexcept;

        //-------------------------------------------------
        // Raise / lower tile vertices
        //-------------------------------------------------

        /**
         * Raises all vertices of this Tile by the value RAISE_Y.
         */
        void Raise();

        /**
         * Lower all vertices of this Tile by the value RAISE_Y.
         */
        void Lower();

        //-------------------------------------------------
        // Normal
        //-------------------------------------------------

        /**
         * Updates the normals of each vertex.
         */
        void UpdateNormal();

        //-------------------------------------------------
        // TileType
        //-------------------------------------------------

        /**
         * Sets tile type and updates the vertices using the new type.
         */
        void UpdateTileType(TileType t_tileType);

        //-------------------------------------------------
        // Selected
        //-------------------------------------------------

        /**
         * Sets the selected state of this Tile.
         */
        void UpdateSelected(bool t_selected);

        //-------------------------------------------------
        // Gpu
        //-------------------------------------------------

        /**
         * Provides the vertices to the Gpu.
         *
         * @param t_vao A Vao object.
         */
        void VerticesToGpu(const ogl::buffer::Vao& t_vao) const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        /**
         * Renders some info about the Tile.
         */
        void RenderImGui() const;

    protected:

    private:
        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the Tile.
         */
        void Init();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        /**
         * Creates an unique color by mapIndex.
         */
        void CreateMapIndexColor();

        /**
         * Computes a normal from all vertices.
         *
         * @return glm::vec3 The normal.
         */
        [[nodiscard]] glm::vec3 CalcNormal() const;
    };
}
