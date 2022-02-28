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

#include "Layer.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::input
{
    class PickingTexture;
}

namespace sg::gui
{
    enum class Action;
}

//-------------------------------------------------
// TerrainLayer
//-------------------------------------------------

namespace sg::map
{
    /**
     * Represents the TerrainLayer.
     */
    class TerrainLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TerrainLayer() = delete;

        /**
         * Constructs a new TerrainLayer object.
         *
         * @param t_tileCount The number of tiles in x and z direction.
         */
        explicit TerrainLayer(int t_tileCount);

        TerrainLayer(const TerrainLayer& t_other) = delete;
        TerrainLayer(TerrainLayer&& t_other) noexcept = delete;
        TerrainLayer& operator=(const TerrainLayer& t_other) = delete;
        TerrainLayer& operator=(TerrainLayer&& t_other) noexcept = delete;

        ~TerrainLayer() noexcept override;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] const auto& GetPickingTexture() const { return *m_pickingTexture; }

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Input();
        void Update(gui::Action t_action, int t_currentTileIndex);
        void RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera);
        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const override;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * An object holding the Fbo for mouse picking.
         */
        std::unique_ptr<ogl::input::PickingTexture> m_pickingTexture;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the class.
         */
        void Init();

        /**
         * Create all Tiles.
         */
        void CreateTiles();

        /**
         * Finds the neighbors for every Tile.
         */
        void AddTileNeighbors() const;

        /**
         * Stores vertices of all Tiles in a Vbo.
         */
        void TilesToGpu();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void UpdateNorthNeighbor(const Tile& t_tile) const;
        void UpdateSouthNeighbor(const Tile& t_tile) const;
        void UpdateWestNeighbor(const Tile& t_tile) const;
        void UpdateEastNeighbor(const Tile& t_tile) const;

        void UpdateNorthWestNeighbor(const Tile& t_tile) const;
        void UpdateNorthEastNeighbor(const Tile& t_tile) const;
        void UpdateSouthWestNeighbor(const Tile& t_tile) const;
        void UpdateSouthEastNeighbor(const Tile& t_tile) const;
    };
}
