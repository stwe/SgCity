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

#include <memory>
#include "ogl/resource/Skybox.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::gui
{
    enum class Action;
}

//-------------------------------------------------
// Map
//-------------------------------------------------

namespace sg::map
{
    /**
     * Forward declaration class WaterLayer.
     */
    class WaterLayer;

    /**
     * Forward declaration class TerrainLayer.
     */
    class TerrainLayer;

    /**
     * Forward declaration class RoadsLayer.
     */
    class RoadsLayer;

    /**
     * Forward declaration class BuildingsLayer.
     */
    class BuildingsLayer;

    /**
     * Forward declaration class PlantsLayer.
     */
    class PlantsLayer;

    /**
     * Forward declaration class Tile.
     */
    class Tile;

    /**
     * Represents the Map.
     */
    class Map
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
         * Index in std::vector<std::shared_ptr<Tile>> of the current picked Tile.
         */
        int currentTileIndex{ INVALID_TILE_INDEX };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Map() = delete;

        /**
         * Constructs a new Map object.
         *
         * @param t_window The Window object.
         * @param t_tileCount The number of tiles in x and z direction.
         */
        Map(std::shared_ptr<ogl::Window> t_window, int t_tileCount);

        Map(const Map& t_other) = delete;
        Map(Map&& t_other) noexcept = delete;
        Map& operator=(const Map& t_other) = delete;
        Map& operator=(Map&& t_other) noexcept = delete;

        ~Map() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] ogl::Window& GetWindow() const { return *m_window; }

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update(gui::Action t_action);
        void RenderForMousePicking(const ogl::camera::Camera& t_camera) const;
        void RenderForWater(
            ogl::camera::Camera& t_camera,
            const ogl::resource::Skybox& t_skybox
            ) const;
        void Render(const ogl::camera::Camera& t_camera) const;
        void RenderImGui() const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The Window object.
         */
        std::shared_ptr<ogl::Window> m_window;

        /*
         * The number of tiles in x and z direction.
         */
        int m_tileCount{ 0 };

        /**
         * The water Layer.
         */
        std::unique_ptr<WaterLayer> m_waterLayer;

        /**
         * The terrain Layer.
         */
        std::unique_ptr<TerrainLayer> m_terrainLayer;

        /**
         * The roads Layer
         */
        std::unique_ptr<RoadsLayer> m_roadsLayer;

        /**
         * The buildings Layer.
         */
        std::unique_ptr<BuildingsLayer> m_buildingsLayer;

        /**
         * The plants layer.
         */
        std::unique_ptr<PlantsLayer> m_plantsLayer;

        /**
         * Current Tile object.
         */
        std::shared_ptr<Tile> m_currentTile;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the Map.
         */
        void Init();

        /**
         * Adds event handlers.
         */
        void InitListeners();

        //-------------------------------------------------
        // Mouse picking
        //-------------------------------------------------

        void UpdateCurrentTileIndex();
    };
}
