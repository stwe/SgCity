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
#include "ogl/buffer/WaterFbos.h"

//-------------------------------------------------
// WaterLayer
//-------------------------------------------------

namespace sg::map
{
    /**
     * Creates and renders the water.
     */
    class WaterLayer : public Layer
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * The sea level.
         */
        static constexpr auto WATER_HEIGHT{ 0.8f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        WaterLayer() = delete;
        explicit WaterLayer(int t_tileCount);

        WaterLayer(const WaterLayer& t_other) = delete;
        WaterLayer(WaterLayer&& t_other) noexcept = delete;
        WaterLayer& operator=(const WaterLayer& t_other) = delete;
        WaterLayer& operator=(WaterLayer&& t_other) noexcept = delete;

        ~WaterLayer() noexcept override;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        [[nodiscard]] const ogl::buffer::WaterFbos& GetWaterFbos() const { return *m_waterFbos; }

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
        void Update() override;

        /**
         * Updates a Layer tile.
         *
         * @param t_action Indicates what should be done with the given tile.
         * @param t_tile The tile.
         */
        void UpdateTile(gui::Action t_action, Tile& t_tile) override {}

        /**
         * Render the Layer.
         *
         * @param t_window The Window object.
         * @param t_camera The Camera object.
         * @param t_plane The clipping plane.
         */
        void Render(
            const ogl::Window& t_window,
            const ogl::camera::Camera& t_camera,
            const glm::vec4& t_plane = glm::vec4(0.0f)
        ) const override;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::unique_ptr<ogl::buffer::WaterFbos> m_waterFbos;
        float m_moveFactor{ 0.0f };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();

        //-------------------------------------------------
        // Override
        //-------------------------------------------------

        /**
         * Creates Layer tiles.
         */
        void CreateTiles() override {}
    };
}
