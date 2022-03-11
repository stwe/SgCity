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

namespace sg::ogl::resource
{
    class Model;
}

//-------------------------------------------------
// PlantsLayer
//-------------------------------------------------

namespace sg::map
{
    /**
     * Represents the PlantsLayer.
     */
    class PlantsLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        PlantsLayer() = delete;

        /**
         * Constructs a new PlantsLayer object.
         *
         * @param t_tiles The Tile objects.
         */
        explicit PlantsLayer(std::vector<std::shared_ptr<Tile>> t_tiles);

        PlantsLayer(const PlantsLayer& t_other) = delete;
        PlantsLayer(PlantsLayer&& t_other) noexcept = delete;
        PlantsLayer& operator=(const PlantsLayer& t_other) = delete;
        PlantsLayer& operator=(PlantsLayer&& t_other) noexcept = delete;

        ~PlantsLayer() noexcept override;

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

        void CreateTiles() override {}
    };
}
