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

#include "Application.h"
#include "ogl/resource/Skybox.h"

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
     * Represents the Map.
     */
    class Map
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /*
         * The number of tiles in x and z direction.
         */
        inline static const auto TILE_COUNT{ Application::INI.Get<int>("world", "tile_count") };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The Window object.
         */
        std::shared_ptr<ogl::Window> window;

        /**
         * The Terrain Layer.
         */
        std::unique_ptr<TerrainLayer> terrainLayer;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Map() = delete;

        /**
         * Constructs a new Map object.
         *
         * @param t_window The Window object.
         */
        explicit Map(std::shared_ptr<ogl::Window> t_window);

        Map(const Map& t_other) = delete;
        Map(Map&& t_other) noexcept = delete;
        Map& operator=(const Map& t_other) = delete;
        Map& operator=(Map&& t_other) noexcept = delete;

        ~Map() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();
        void RenderForMousePicking(const ogl::camera::Camera& t_camera) const;
        void RenderForWater(ogl::camera::Camera& t_camera, const ogl::resource::Skybox& t_skybox) const;
        void Render(const ogl::camera::Camera& t_camera) const;
        void RenderImGui() const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The Water Layer.
         */
        std::unique_ptr<WaterLayer> m_waterLayer;

        /**
         * The Roads Layer
         */
        std::unique_ptr<RoadsLayer> m_roadsLayer;

        /**
         * The Buildings Layer.
         */
        std::unique_ptr<BuildingsLayer> m_buildingsLayer;

        /**
         * The Plants layer.
         */
        std::unique_ptr<PlantsLayer> m_plantsLayer;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        /**
         * Initializes the Map.
         */
        void Init();

        /**
         * Initializes the event dispatcher.
         */
        void InitEventDispatcher() const;
    };
}
