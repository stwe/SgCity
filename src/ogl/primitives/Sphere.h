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

#include "ogl/camera/Camera.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::buffer
{
    class Vao;
}

//-------------------------------------------------
// Sphere
//-------------------------------------------------

namespace sg::ogl::primitives
{
    /**
     * Represents a sphere.
     */
    class Sphere
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Sphere() = delete;

        /**
         * Constructs a new Sphere object.
         *
         * @param t_window The Window object.
         * @param t_radius The radius of the sphere.
         * @param t_slices The number of horizontal slices.
         * @param t_stacks The number of vertical stacks.
         */
        Sphere(std::shared_ptr<Window> t_window, float t_radius, int t_slices, int t_stacks);

        Sphere(const Sphere& t_other) = delete;
        Sphere(Sphere&& t_other) noexcept = delete;
        Sphere& operator=(const Sphere& t_other) = delete;
        Sphere& operator=(Sphere&& t_other) noexcept = delete;

        ~Sphere() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        /**
         * Render the sphere.
         *
         * @param t_camera The Camera object.
         * @param t_position The position of this sphere.
         * @param t_rotation The rotation of this sphere.
         * @param t_scale The scale of this sphere.
         */
        void Render(
            const camera::Camera& t_camera,
            const glm::vec3& t_position,
            const glm::vec3& t_rotation = glm::vec3(0.0f),
            const glm::vec3& t_scale = glm::vec3(1.0f)
        ) const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The Window object.
         */
        std::shared_ptr<Window> m_window;

        /**
         * The Vao of this sphere.
         */
        std::unique_ptr<buffer::Vao> m_sphereVao;
    };
}
