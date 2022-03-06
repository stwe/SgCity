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
#include "BuildingTile.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::resource
{
    class Model;
}

//-------------------------------------------------
// BuildingsLayer
//-------------------------------------------------

namespace sg::map
{
    class BuildingsLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        BuildingsLayer() = delete;
        BuildingsLayer(int t_tileCount, std::vector<std::shared_ptr<Tile>> t_tiles);

        BuildingsLayer(const BuildingsLayer& t_other) = delete;
        BuildingsLayer(BuildingsLayer&& t_other) noexcept = delete;
        BuildingsLayer& operator=(const BuildingsLayer& t_other) = delete;
        BuildingsLayer& operator=(BuildingsLayer&& t_other) noexcept = delete;

        ~BuildingsLayer() noexcept override;

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        void Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera) const override;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * A Model object.
         */
        std::shared_ptr<ogl::resource::Model> m_model;

        /**
         * An array holding the BuildingTile objects.
         */
        std::vector<std::shared_ptr<BuildingTile>> m_buildingTiles;

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
         * Create tiles.
         */
        void CreateTiles() override;

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        /**
         * Stores vertices in a Vbo.
         */
        void BuildingTilesToGpu();

        static std::unique_ptr<BuildingTile> CreateBuildingTile(const Tile& t_tile, int t_index);
    };
}
