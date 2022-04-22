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
#include "RoadTile.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::gui
{
    enum class Action;
}

//-------------------------------------------------
// RoadsLayer
//-------------------------------------------------

namespace sg::map
{
    /**
     * Represents the RoadsLayer.
     */
    class RoadsLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        RoadsLayer() = delete;

        RoadsLayer(std::shared_ptr<ogl::Window> t_window, int t_tileCount, std::vector<std::shared_ptr<Tile>> t_tiles);

        RoadsLayer(const RoadsLayer& t_other) = delete;
        RoadsLayer(RoadsLayer&& t_other) noexcept = delete;
        RoadsLayer& operator=(const RoadsLayer& t_other) = delete;
        RoadsLayer& operator=(RoadsLayer&& t_other) noexcept = delete;

        ~RoadsLayer() noexcept override;

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
         * Render the Layer.
         *
         * @param t_camera The Camera object.
         * @param t_plane The clipping plane.
         */
        void Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane = glm::vec4(0.0f)) override;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * An array holding the RoadTile objects.
         */
        std::vector<std::shared_ptr<RoadTile>> m_roadTiles;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the class.
         */
        void Init();

        /**
         * Initializes the event dispatcher.
         */
        void InitEventDispatcher();

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        /**
         * Creates Layer tiles.
         */
        void CreateTiles() override;

        //-------------------------------------------------
        // Listeners
        //-------------------------------------------------

        /**
         * On create road event handler.
         */
        void OnCreateRoad(const Tile& t_tile);

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        /**
         * Stores vertices of all RoadTiles in a Vbo.
         */
        void RoadTilesToGpu();

        static std::unique_ptr<RoadTile> CreateRoadTile(const Tile& t_tile, int t_index);

        static void UpdateTexture(RoadTile& t_roadTile);

        static bool CheckTerrainForRoad(const Tile& t_tile);
    };
}
