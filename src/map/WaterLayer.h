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
        static constexpr auto WATER_HEIGHT{ 0.1f };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        WaterLayer() = delete;

        WaterLayer(int t_tileCount, std::shared_ptr<ogl::Window> t_window);

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
         * Render the Layer.
         *
         * @param t_camera The Camera object.
         * @param t_plane The clipping plane.
         */
        void Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane) override;

        /**
         * Renders an ImGui window.
         */
        void RenderImGui() override;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The number of tiles in x and z direction.
         */
        int m_tileCount;

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
