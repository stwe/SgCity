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
#include "gui/MapEditGui.h"

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
        // Constants
        //-------------------------------------------------

        /**
         * Value used for an invalid tile index.
         */
        static constexpr auto INVALID_TILE_INDEX{ -1 };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * Index in Tile::std::vector<std::shared_ptr<Tile>> of the current picked Tile.
         */
        int currentTileIndex{ INVALID_TILE_INDEX };

        /**
         * An object holding the Fbo for mouse picking.
         */
        std::unique_ptr<ogl::input::PickingTexture> pickingTexture;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        TerrainLayer() = delete;

        /**
         * Constructs a new TerrainLayer object.
         *
         * @param t_window The Window object.
         * @param t_tileCount The number of tiles in x and z direction.
         */
        TerrainLayer(std::shared_ptr<ogl::Window> t_window, int t_tileCount);

        TerrainLayer(const TerrainLayer& t_other) = delete;
        TerrainLayer(TerrainLayer&& t_other) noexcept = delete;
        TerrainLayer& operator=(const TerrainLayer& t_other) = delete;
        TerrainLayer& operator=(TerrainLayer&& t_other) noexcept = delete;

        ~TerrainLayer() noexcept override;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void RenderForMousePicking(const ogl::Window& t_window, const ogl::camera::Camera& t_camera);

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        /**
         * Input handling.
         */
        void Input() override {}

        /**
         * Updates the Layer.
         */
        void Update() override {}

        /**
         * Updates a Layer tile.
         *
         * @param t_action Indicates what should be done with the given tile.
         * @param t_tile The tile.
         */
        void UpdateTile(gui::Action t_action, Tile& t_tile) override;

        /**
         * Render the Layer.
         *
         * @param t_camera The Camera object.
         * @param t_plane The clipping plane.
         */
        void Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane = glm::vec4(0.0f)) const override;

        /**
         * Renders an ImGui window.
         */
        void RenderImGui() override;

        //-------------------------------------------------
        // Listeners
        //-------------------------------------------------

        void OnLeftMouseButtonPressed() override;
        void OnLeftMouseButtonReleased() override;
        void OnMouseMoved() override;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The current Tile object.
         */
        std::shared_ptr<Tile> m_currentTile;

        /**
         * Shows the terrain editing menu.
         */
        gui::MapEditGui m_mapEditGui;

        /**
         * Helper flag for selecting tiles.
         */
        bool m_selectFlag{ false };

        /**
         * The index of the first selected tile.
         */
        int m_startIndex{ INVALID_TILE_INDEX };

        /**
         * The index of the last selected tile.
         */
        int m_endIndex{ INVALID_TILE_INDEX };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the class.
         */
        void Init();

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        /**
         * Creates Layer tiles.
         */
        void CreateTiles() override;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        /**
         * Finds the neighbors for every Tile.
         */
        void AddTileNeighbors() const;

        /**
         * Stores vertices of all Tiles in a Vbo.
         */
        void TilesToGpu();

        void UpdateTileVertices(Tile& t_tile) const;

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
