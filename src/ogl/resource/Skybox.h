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

#include <vector>
#include <cstdint>
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"

//-------------------------------------------------
// Skybox
//-------------------------------------------------

namespace sg::ogl::resource
{
    /**
     * Represents a Skybox.
     */
    class Skybox
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        uint32_t id{ 0 };
        uint32_t skyboxVao{ 0 }, skyboxVbo{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Skybox();

        Skybox(const Skybox& t_other) = delete;
        Skybox(Skybox&& t_other) noexcept = delete;
        Skybox& operator=(const Skybox& t_other) = delete;
        Skybox& operator=(Skybox&& t_other) noexcept = delete;

        ~Skybox();

        //-------------------------------------------------
        // Bind
        //-------------------------------------------------

        void Bind() const;
        void BindForReading() const;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Render(const Window& t_window, const camera::Camera& t_camera) const;

    protected:

    private:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        inline static const std::vector<std::string> FACES
        {
            "E:/Dev/SgCity/resources/skybox/right.jpg",
            "E:/Dev/SgCity/resources/skybox/left.jpg",
            "E:/Dev/SgCity/resources/skybox/top.jpg",
            "E:/Dev/SgCity/resources/skybox/bottom.jpg",
            "E:/Dev/SgCity/resources/skybox/front.jpg",
            "E:/Dev/SgCity/resources/skybox/back.jpg"
        };

        inline static const std::vector<float> SKYBOX_VERTICES
        {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        //-------------------------------------------------
        // Create
        //-------------------------------------------------

        void CreateId();
        void CreateBuffer();

        //-------------------------------------------------
        // Load
        //-------------------------------------------------

        /**
         * Loads a cubemap texture from 6 individual texture faces.
         * order:
         * +X (right)
         * -X (left)
         * +Y (top)
         * -Y (bottom)
         * +Z (front)
         * -Z (back)
         */
        void LoadFaces();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp() const;
    };
}
